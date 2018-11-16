#include "stm32f4xx.h"

// TIM2 configure
void timerInit() {
  
  // Enable TIM2 clock
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
  TIM2->PSC = 24000 - 1;
  TIM2->ARR = 1000;
  TIM2->DIER |= TIM_DIER_UIE;
  TIM2->CR1 |= TIM_CR1_CEN;
  
  NVIC_EnableIRQ(TIM2_IRQn);
}


int main(void) {
  
  // Enable GPIO clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  // GPIO Mode: Output (01)
  GPIOA->MODER &= ~(GPIO_MODER_MODE5);
  GPIOA->MODER |= GPIO_MODER_MODE5_0;
  
  // Output: push-pull
  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_5);
  
  timerInit();
  
  while(1);
}


// Interrupt handler for TIM2
void TIM2_IRQHandler(void) {
  
  // Cleared flag
  TIM2->SR &= ~TIM_SR_UIF;
  
  // Toggle LED
  GPIOA->ODR ^= GPIO_ODR_OD5;
}
