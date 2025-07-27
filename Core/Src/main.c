#include "stm32f4xx.h"
#include<stdint.h>
#define GPIOAEN (1U<<0)
#define UART_EN (1U<<17)
#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUD_RATE 115200
#define CR1_TE (1U<<3)
#define SR_TXE (1U<<7)
static uint16_t compute_uart_div(uint32_t Perif_clk,uint32_t Baudrate);
void UART2_TX_INIT(void);
void uart2_write(int ch);
static  void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Perif_clk,uint32_t Baudrate);
static  void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t Perif_clk,uint32_t Baudrate){
	USARTx->BRR=compute_uart_div(Perif_clk,Baudrate);
}




//calculate the baud rate 0
static uint16_t compute_uart_div(uint32_t Perif_clk,uint32_t Baudrate){
	return ((Perif_clk +(Baudrate/2U))/Baudrate);

}



void UART2_TX_INIT(void){
	/*************congiure uart gpio pin*************/
	//enable clock to gpioa
	RCC->AHB1ENR|=GPIOAEN;
	//set pa2 to alternate function mode
	GPIOA->MODER|=(1U<<5);
	GPIOA->MODER&=~(1U<<4);
	//set pa2 alternate function type to UART_TX(AF07)
	GPIOA->AFR[0]|=(1U<<8);//for afrl low arf[0] and for afr high arf[1]
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);




	/*configure uart module*/
	//enable clock  to uart2
	RCC->APB1ENR|=UART_EN;
	//configure baud rate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUD_RATE);
	//configure the transfer direction
	USART2->CR1 =CR1_TE;
	USART2->CR1|=(1U<<13);
	//enable uart module
}


void uart2_write(int ch){
	//make sure the transmit data register is empty
	while(!(USART2->SR & SR_TXE)){}
	//write to transmit data register
	USART2->DR = ch & 0xff;

}

int main(){

	UART2_TX_INIT();

	while(1){
		uart2_write('N');
	}
}




