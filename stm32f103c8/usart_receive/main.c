#include "stm32f10x.h"

uint8_t buffer;

// Print char to serial port
void serialReceiveChar () {

	if (USART1->SR & USART_SR_RXNE) {
		
		buffer = USART1->DR;
	}
}

void UARTInit() {

	// Enable USART#1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// BRR = (fck + baudrate /2 ) / baudrate
  // fck = 72MHz, baudrate = 9600
	USART1->BRR = 0x1d4c;
	
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
	
	// Receiver enable
	USART1->CR1 |= USART_CR1_RE;
	
	// Enable GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Setup PA10 as RX
	// GPIO Mode: Alternative function
	GPIOA->CRH &= ~(GPIO_CRH_MODE10);
	
	// Input floating mode
	GPIOA->CRH &= ~(GPIO_CRH_CNF10);
	GPIOA->CRH |= GPIO_CRH_CNF10_0;
	
}


void delay(int milisec){
	for (int i=0; i<7200*milisec; i++);  // приблизительно 1сек
}


int main(void) {
	
	UARTInit();
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13);
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	GPIOC->CRH |= GPIO_CRH_MODE13_0;
	
	GPIOC->CRH &= ~(GPIO_CRH_CNF13);
	
	GPIOC->ODR ^= GPIO_ODR_ODR13;
	
  
  while(1) {
		
		serialReceiveChar();
		
		if (buffer == '1') {
			GPIOC->ODR ^= GPIO_ODR_ODR13;
			buffer = '0';
		}		
	}
}
