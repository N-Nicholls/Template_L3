/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3A
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "SysTimer.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;  // clear SW bits → selects MSI
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7;
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	// Enable MSI oscillator
	RCC->CR |= RCC_CR_MSION;
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0);
}

int main() {
	// Initialization 
	System_Clock_Init();
	SysTick_Init();
	LED_Init();
	
	while (1) {
    	Green_LED_Toggle();
    	delay(1000);
	}
}
