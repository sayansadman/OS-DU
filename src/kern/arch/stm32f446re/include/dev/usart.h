#ifndef __USART_H
#define __USART_H
#include "../sys/stm32_peps.h"
void DRV_USART_INIT(USART_TypeDef*);
void DRV_USART2_RXNE_Enable();
void _USART_WRITE(USART_TypeDef*, uint8_t*);
uint8_t _USART_READ(USART_TypeDef*, uint8_t*, uint16_t);
uint8_t _USART_READ_WORD(USART_TypeDef*, uint8_t*, uint16_t);
void UART_SendChar(USART_TypeDef*, uint8_t);
uint8_t UART_GetChar(USART_TypeDef*);
#endif
