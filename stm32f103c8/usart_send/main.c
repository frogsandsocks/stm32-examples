#include "stm32f10x.h"


// Print char to serial port
void serialPutChar (uint8_t ch) {

	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = (ch);
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


void UARTInit() {

	// Enable USART#1 clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// BRR = (fck + baudrate /2 ) / baudrate
  // fck = 72MHz, baudrate = 9600
	USART1->BRR = 0x1d4c;
	
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
	
	// Transmitter enable
	USART1->CR1 |= USART_CR1_TE;
	
	// Enable GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Setup PA9 as TX
	// GPIO Mode: Alternative function
	GPIOA->CRH &= ~(GPIO_CRH_MODE9);
	GPIOA->CRH |= GPIO_CRH_MODE9_0;
	GPIOA->CRH |= GPIO_CRH_MODE9_1;
	
	// Change if wont be work
	GPIOA->CRH &= ~(GPIO_CRH_CNF9);
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	
}


void delay(int milisec){
	for (int i=0; i<7200*milisec; i++);  // приблизительно 1сек
}


int main(void) {
	
	UARTInit();
  
  while(1) {
		
		serialPrint("Hello World!");
		delay(500);
	}
}
