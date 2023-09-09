/*
 * uart_driver.c
 *
 *  Created on: Sep 9, 2023
 *      Author: brknu
 */
#include "stm32l0xx_hal.h"
#include "uart_driver.h"

UART_HandleTypeDef UartHandle;
uint32_t rx_data = 0;

static void UART_Error_Handler(void);

void UART_Init(){

	GPIO_InitTypeDef GPIO_InitStruct;

	// 1 - Clock Settings
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	// 2 - GPIO Pin Configuration

	// TX Pin Configuration

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_USART2;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	// RX Pin Configuration

	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_USART2;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	// 3 - UART Pin Configuration

	UartHandle.Instance = USART2;
	UartHandle.Init.BaudRate = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	//UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	 if (HAL_UART_Init(&UartHandle) != HAL_OK) {
	        //ERROR
	        UART_Error_Handler();
	 }

	// 4 - Enable UART Receive Data Register is not empty
	SET_BIT(USART2->CR1,USART_CR1_RXNEIE);

	// 5 - Enable UART Interrupt
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

}

static void UART_Error_Handler(void)
{
     while(1){

    }
}

void USART2_IRQHandler(){
	uint32_t isrflag = USART2->ISR;
	uint32_t cr1 = USART2->CR1;

	// UART in Receiver Mode
	if(((isrflag & USART_ISR_RXNE) != RESET) && ((cr1 & USART_CR1_RXNEIE) != RESET)){
		rx_data = (uint8_t) USART2->RDR;
		return;
	}

	// UART in Transmitter mode
	if(((isrflag & USART_ISR_TXE) != RESET) && ((cr1 & USART_CR1_TXEIE) != RESET)){
		return;
	}

}


