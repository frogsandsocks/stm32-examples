#include "stm32f10x.h"

char buffer[] = "Hello";

void DMAInit() {
	
	// Enable DMA1 clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	// Specifies the buffer size, in data unit, of the specified Channel
	DMA1_Channel4->CNDTR = 5;
	
	// Specifies the peripheral base address
	DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
	// Specifies the memory base address
	DMA1_Channel4->CMAR = (uint32_t) buffer;
	
	// Specifies the Peripheral and Memory data width
	// 00: 8-bits
	// 01: 16-bits
	// 10: 32-bits
	DMA1_Channel4->CCR &= ~(DMA_CCR4_PSIZE | DMA_CCR4_MSIZE);
	
	// Specifies if the peripheral is the source or destination
	// 0: Read from peripheral (default)
	// 1: Read from memory
	DMA1_Channel4->CCR |= DMA_CCR4_DIR;
	
	// Specifies whether the memory address register is incremented or not
	// 0: Memory increment mode disabled (default)
	// 1: Memory increment mode enabled  
	DMA1_Channel4->CCR |= DMA_CCR4_MINC;
	
	// Enables or disables interrupt for specified DMA and Channel
	// 0: TC interrupt disabled (default)
	// 1: TC interrupt enabled
	DMA1_Channel4->CCR |= DMA_CCR4_TCIE;
	
	// Enables or disables the specified DMA and Channel
	// 0: Channel disabled (default)
	// 1: Channel enabled
	DMA1_Channel4->CCR |= DMA_CCR4_EN;
	
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}


void UARTInit() {

	// Enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;
	
	// BRR = (fck + baudrate /2 ) / baudrate
  // fck = 72MHz, baudrate = 9600
	USART1->BRR = 0x1d4c;
	
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
	
	// Transmitter enable
	USART1->CR1 |= USART_CR1_TE;
	
	// Transmitter enable for DMA
	USART1->CR3 |= USART_CR3_DMAT;
	
	// Enable GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Setup PA9 as TX
	// GPIO Mode: Alternative function
	GPIOA->CRH &= ~(GPIO_CRH_MODE9);
	GPIOA->CRH |= GPIO_CRH_MODE9_0;
	GPIOA->CRH |= GPIO_CRH_MODE9_1;
	
	
	GPIOA->CRH &= ~(GPIO_CRH_CNF9);
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	
}


int main(void) {
	
	UARTInit();
	DMAInit();
	
	while(1);
}


void DMA1_Channel4_IRQHandler(void) {
	DMA1->IFCR |= DMA_IFCR_CTCIF4;
	DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR4_EN);
}
