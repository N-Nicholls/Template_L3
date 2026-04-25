/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
    // Enable GPIOB clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    
    // PB6 to Alternate Function mode
    GPIOB->MODER &= ~GPIO_MODER_MODE6;
    GPIOB->MODER |=  GPIO_MODER_MODE6_1;
    
    // AF2 = TIM4_CH1 for PB6
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
    GPIOB->AFR[0] |=  GPIO_AFRL_AFSEL6_1;  // AF2 = 0010
    
    // No pull-up, no pull-down
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
    
    // Enable TIM4 clock (APB1)
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
    
    // Count up
    TIM4->CR1 &= ~TIM_CR1_DIR;
    
    // Prescaler: 1MHz
    TIM4->PSC = 15;
    
    // ARR max, enable preload
    TIM4->ARR = 0xFFFF;
    TIM4->CR1 |= TIM_CR1_ARPE;
    
    // CCMR1: CC1S = 01 (input, IC1 mapped to TI1)
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM4->CCMR1 |=  TIM_CCMR1_CC1S_0;
    
    // Capture both rising and falling edges (CC1P=1, CC1NP=1)
    TIM4->CCER |= TIM_CCER_CC1P;
    TIM4->CCER |= TIM_CCER_CC1NP;
    
    // Enable capture
    TIM4->CCER |= TIM_CCER_CC1E;
    
    // Enable CC1 interrupt, DMA, and update interrupt
    TIM4->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC1DE | TIM_DIER_UIE;
    
    // Force update event
    TIM4->EGR |= TIM_EGR_UG;
    
    // Clear update flag
    TIM4->SR &= ~TIM_SR_UIF;
    
    // Enable counter
    TIM4->CR1 |= TIM_CR1_CEN;
    
    // Enable TIM4 interrupt in NVIC, priority 2
    NVIC_EnableIRQ(TIM4_IRQn);
    NVIC_SetPriority(TIM4_IRQn, 2);
}

void TIM4_IRQHandler(void) {
    // Handle overflow
    if (TIM4->SR & TIM_SR_UIF) {
        overflowCount++;
        TIM4->SR &= ~TIM_SR_UIF;  // must explicitly clear
    }
    
    // Handle capture event
    if (TIM4->SR & TIM_SR_CC1IF) {
        currentValue = TIM4->CCR1;  // reading CCR1 clears CC1IF automatically
        
        if (GPIOB->IDR & GPIO_IDR_ID6) {
            // Pin is high → this was a rising edge, record start
            lastValue = currentValue;
            overflowCount = 0;
        } else {
            // Pin is low → this was a falling edge, compute pulse width
            if (overflowCount == 0) {
                timeInterval = currentValue - lastValue;
            } else {
                // Account for overflows
                timeInterval = (0xFFFF - lastValue) + currentValue 
                               + (overflowCount - 1) * 0xFFFF;
            }
        }
    }
}

void Trigger_Setup() {
    // Enable GPIOA clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // PA9 to Alternate Function mode
    GPIOA->MODER &= ~GPIO_MODER_MODE9;
    GPIOA->MODER |=  GPIO_MODER_MODE9_1;
    
    // AF1 = TIM1_CH2 for PA9 (check your AF table)
    GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
    GPIOA->AFR[1] |=  GPIO_AFRH_AFSEL9_0;  // AF1 = 0001
    
    // No pull-up, no pull-down
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
    
    // Push-pull output
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
    
    // Very high speed
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;
    
    // Enable TIM1 clock (on APB2)
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    
    // Count up
    TIM1->CR1 &= ~TIM_CR1_DIR;
    
    // Prescaler: 16MHz / (15+1) = 1MHz
    TIM1->PSC = 15;
    
    // ARR max value, enable preload
    TIM1->ARR = 0xFFFF;
    TIM1->CR1 |= TIM_CR1_ARPE;
    
    // CCR2 = 10 → 10µs pulse width
    TIM1->CCR2 = 10;
    
    // CCMR1: CC2S = 00 (output), OC2M = 110 (PWM mode 1), OC2PE = 1
    TIM1->CCMR1 &= ~TIM_CCMR1_CC2S;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
    TIM1->CCMR1 |=  TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;
    TIM1->CCMR1 |=  TIM_CCMR1_OC2PE;
    
    // Enable CH2 output, active high
    TIM1->CCER &= ~TIM_CCER_CC2P;
    TIM1->CCER |=  TIM_CCER_CC2E;
    
    // TIM1 is an advanced timer — needs MOE, OSSI, OSSR set in BDTR
    TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSI | TIM_BDTR_OSSR;
    
    // Force update event to load preload registers
    TIM1->EGR |= TIM_EGR_UG;
    
    // Enable update interrupt and clear flag
    TIM1->DIER |= TIM_DIER_UIE;
    TIM1->SR &= ~TIM_SR_UIF;
    
    // Enable counter
    TIM1->CR1 |= TIM_CR1_CEN;
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	
	while(1) {
		uint32_t volatile distance_cm = timeInterval / 58;
		
	}
}
