#include "stm32f4xx.h"

int main(void) {
	
	// Enable GPIO Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	// GPIO Mode for LED on PA5: Output (01)
	GPIOA->MODER &= ~(GPIO_MODER_MODE5);
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5);
	
	// No pull-up, pull-down (00)
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR5);
	
	
	// GPIO Mode for button on PC13: Input (00)
	GPIOC->MODER &= ~(GPIO_MODER_MODE13);
	
	// Pull-down (10)
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR13);
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_1;
	
	// Enable Interrupt
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI13);
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	// Enable interrupt for 13 pin
	EXTI->IMR |= EXTI_IMR_MR13;
	
	// Falling trigger selection
	EXTI->FTSR |= EXTI_FTSR_TR13;
	
	// Rising trigger selection
	EXTI->RTSR |= EXTI_RTSR_TR13;
	
	while(1);
}


// Interrupt handler for EXTI9_5
void EXTI15_10_IRQHandler(void) {
	
	// Cleared flag
	EXTI->PR |= EXTI_PR_PR13;
	
	// Toggle LED
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
