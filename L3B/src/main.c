/*
 * ECE 153B
 * 
 * Name(s):
 * Section:
 * Lab: 3B
 */

#include "stm32l476xx.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);
void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);

void LED_Pin_Init(void){
  	// Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;


	// Initialize Green LED
	// GPIO Mode
	GPIOA->MODER &= ~GPIO_MODER_MODE5;
    GPIOA->MODER |=  GPIO_MODER_MODE5_1; // (10) Alternate Function Mode

	// Alternative Function
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;       // clear all 4 bits
	GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL5_0;    // set bit 0 → AF1 = 0001

	//Set I/O output speed value as very high speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;

	// GPIO Push-Pull: No pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;

}

void TIM2_CH1_Init(void){
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR;
  	// Prescaler 
	TIM2->PSC = 3; // 4MHz / (3+1) = 1MHz
  	// Auto-reload
	TIM2->ARR = 999; // 1kHz PWM
	// Disable output compare mode
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM2->CCMR1 |=  TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	// Select output polarity: active high
	TIM2->CCER &= ~TIM_CCER_CC1P;
  	// Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;  // output mode
	TIM2->CCR1 = 500; // 50% duty cycle
	// Output Compare Register for channel 1 
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN;

}

void SERVO_Pin_Init(void) {
    // GPIOA already enabled in LED_Pin_Init, but be safe
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    // PA0 to Alternate Function mode (10)
    GPIOA->MODER &= ~GPIO_MODER_MODE0;
    GPIOA->MODER |=  GPIO_MODER_MODE0_1;
    
    // Select AF2 (TIM5_CH1) for PA0
    // PA0 → bits [3:0] → shift by 0
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;
	GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL0_1;  // bit 1 → AF2 = 0010


	
    
    // Very high speed
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0;
    
    // No pull-up, no pull-down
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
}

void TIM5_CH1_Init(void) {
    // Enable TIM5 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
    
    // Count up
    TIM5->CR1 &= ~TIM_CR1_DIR;
    
    // Prescaler: 1MHz timer clock
    TIM5->PSC = 3;
    
    // Auto-reload: 20ms period = 50Hz
    TIM5->ARR = 19999;
    
    // PWM mode 1
    TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM5->CCMR1 |=  TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    
    // Enable output preload
    TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
    
    // Active high
    TIM5->CCER &= ~TIM_CCER_CC1P;
    
    // Enable CH1 output
    TIM5->CCER |= TIM_CCER_CC1E;

	TIM5->CCMR1 &= ~TIM_CCMR1_CC1S;
    
    // Start at center (1.5ms)
    TIM5->CCR1 = 1500;
    
    // Enable counter
    TIM5->CR1 |= TIM_CR1_CEN;
}

int main(void) {
	volatile int i;
	int led = 0;
	int dire = 1;
	
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
		
    while (1) {
		if (dire == 1) {
			led++;
			if (led >= 999) dire = -1;
		} else {
			led--;
			if (led <= 0) dire = 1;
		}
		
		TIM2->CCR1 = led;
		TIM5->CCR1 = 1000 + led;  // maps 0-999 → 1000-1999ms pulse
		
		for (i = 0; i < 3000; i++);  // tune this for smooth speed
	}
}
