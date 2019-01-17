#include "stm32f10x.h"

uint8_t buffer;

void DMAInit() {
	
	// Enable DMA1 clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	
	// Specifies the buffer size, in data unit, of the specified Channel
	DMA1_Channel5->CNDTR = 1;
	
	// Specifies the peripheral base address
	DMA1_Channel5->CPAR = (uint32_t) &(USART1->DR);
	// Specifies the memory base address
	DMA1_Channel5->CMAR = (uint32_t) &buffer;
	
	// Specifies the Peripheral and Memory data width
	// 00: 8-bits
	// 01: 16-bits
	// 10: 32-bits
	DMA1_Channel5->CCR &= ~(DMA_CCR5_PSIZE | DMA_CCR5_MSIZE);
	
	// Specifies if the peripheral is the source or destination
	// 0: Read from peripheral (default)
	// 1: Read from memory
	DMA1_Channel5->CCR &= ~(DMA_CCR5_DIR);
	
	DMA1_Channel5->CCR |= DMA_CCR5_CIRC;
	
	// Specifies whether the memory address register is incremented or not
	// 0: Memory increment mode disabled (default)
	// 1: Memory increment mode enabled  
	//DMA1_Channel5->CCR |= DMA_CCR5_MINC;
	
	// Enables or disables interrupt for specified DMA and Channel
	// 0: TC interrupt disabled (default)
	// 1: TC interrupt enabled
	DMA1_Channel5->CCR |= DMA_CCR5_TCIE;
	
	// Enables or disables the specified DMA and Channel
	// 0: Channel disabled (default)
	// 1: Channel enabled
	DMA1_Channel5->CCR |= DMA_CCR5_EN;
}


void UARTInit() {

	// Enable USART1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;
	
	// BRR = (fck + baudrate /2 ) / baudrate
	// fck = 72MHz, baudrate = 9600
	USART1->BRR = 0x1d4c;
	
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
	
	// Receiver enable
	USART1->CR1 |= USART_CR1_RE;
	
	USART1->CR3 |= USART_CR3_DMAR;
	
	// Enable GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Setup PA10 as RX
	// GPIO Mode: Input
	GPIOA->CRH &= ~(GPIO_CRH_MODE10);
	
	// Input floating mode
	GPIOA->CRH &= ~(GPIO_CRH_CNF10);
	GPIOA->CRH |= GPIO_CRH_CNF10_0;
	
}


int main(void) {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13);
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	GPIOC->CRH |= GPIO_CRH_MODE13_0;
	
	GPIOC->CRH &= ~(GPIO_CRH_CNF13);
	
	
	DMAInit();
	UARTInit();
	
	while(1);
}


void DMA1_Channel5_IRQHandler(void) {
	
	DMA1->IFCR |= DMA_IFCR_CTCIF5;
	
	if (buffer == 'h') {
		GPIOC->ODR ^= GPIO_ODR_ODR13;
		buffer = '0';
	}
}
