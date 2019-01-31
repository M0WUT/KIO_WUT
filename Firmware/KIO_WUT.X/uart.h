/* 
 * File: uart.h   
 * Author: Dan McGraw M0WUT
 */
 
#ifndef UART_H
#define	UART_H

#include <xc.h>

typedef struct uart{
    uint16_t number; // Which number UART this is  
    uint16_t txPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint16_t rxPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint32_t baudrate; // Have a guess :) 
    uint16_t stopbits;
    uint16_t *breg; // Base address for UART's config registers, just to save recalculating every time
    char rxChar; //Received character for top level program to handle
} uart_t;


void setup_uart(uart_t uart);
char rx_char(uart_t uart);
void tx_char (uart_t uart, char a);



#endif	// UART_H 

