#include "stm32f4xx.h"


/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/


void SystemCoreClockConfigure(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock
	
  FLASH->ACR  = FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY_5WS;                     // Flash 5 wait state

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL

	
  // PLL configuration:  VCO = HSI/M * N,  Sysclk = VCO/P
  
	RCC->PLLCFGR = ( 16ul                   |                // PLL_M =  16
                 (384ul <<  6)            |                // PLL_N = 384
                 (  3ul << 16)            |                // PLL_P =   8
                 (RCC_PLLCFGR_PLLSRC_HSI) |                // PLL_SRC = HSI
                 (  8ul << 24)             );              // PLL_Q =   8

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}


// Print char to serial port
void serialPutChar (uint8_t ch) {
	
  while (!(USART2->SR & USART_SR_TC));
  USART2->DR = (ch);
}


// Print string to serial port
void serialPrint (char * string) {
	uint8_t i = 0;
	
	while (string[i]) {
		serialPutChar(string[i]);
		i++;
	}
	
	serialPutChar('\r');
	serialPutChar('\n');
}


// TIM2 configure
void timerInit() {
	
	// Enable TIM2 clock
	RCC->APB1ENR  |=   RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 48000 - 1;
	TIM2->ARR = 1000;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM2_IRQn);
}


// UART configure at 115200 baud
void UARTInit() {
	
	// Enable USART#2 clock
	RCC->APB1ENR  |=   RCC_APB1ENR_USART2EN;
	
	// BRR = (fck + baudrate /2 ) / baudrate
	// fck = 48MHz, baudrate = 115200
	USART2->BRR  = 0x1a1;
	
	// USART enable
  USART2->CR1 |= USART_CR1_UE;
	
	// Transmitter enable
	USART2->CR1 |= USART_CR1_TE;
	
	// Enable GPIO clock
	RCC->AHB1ENR  |=   RCC_AHB1ENR_GPIOAEN;
  
	// Setup PA2 as TX
	// GPIO Mode: Alternative function (10)
	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
  
	// AF7 (0111)
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2);
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_2;
}


int main(void) {
	
	// configure HSI as System Clock
	SystemCoreClockConfigure();
  SystemCoreClockUpdate();
	
	timerInit();
	UARTInit();
	
	while(1);
}


// Interrupt handler for TIM2
void TIM2_IRQHandler(void) {

	serialPrint("Hello World!");
	
	// Cleared flag
	TIM2->SR &= ~(TIM_SR_UIF);
}