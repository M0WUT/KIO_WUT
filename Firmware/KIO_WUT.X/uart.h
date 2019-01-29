/* 
 * File: uart.h   
 * Author: Dan McGraw M0WUT
 */
 
#ifndef UART_H
#define	UART_H

#include<xc.h>

typedef struct uart{
    uint16_t number; // Which number UART this is  
    uint16_t txPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint16_t rxPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint32_t baudrate; // Have a guess :) 
    uint16_t stopbits;
}uart_t;


void setup_uart(uart_t x);

#endif	// UART_H 

