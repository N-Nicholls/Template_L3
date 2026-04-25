/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	
	// Initialize Green LED
	GPIOA->MODER &= ~GPIO_MODER_MODE5;    // clears to 00
    GPIOA->MODER |=  GPIO_MODER_MODE5_0;  // sets to 01 (output) using just bit 0

    // set mode type to push-pull
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5; // clears bit to 0 (Push-Pull)

    // set PUPD to no PUPD
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5; // clears bits to 00 (No PUPD)
}

void Green_LED_Off(void) {
	GPIOA->ODR &= ~(1 << 5);
}

void Green_LED_On(void) {
	GPIOA->ODR |= (1 << 5);
}

void Green_LED_Toggle(void){
	GPIOA->ODR ^= (1 << 5);
}