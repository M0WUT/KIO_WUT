/* 
 * File: uart.h   
 * Author: Dan McGraw M0WUT
 */
 
#ifndef UART_H
#define	UART_H

#include<xc.h>

typedef struct uart{
    int number; // Which number UART this is  
    int txPin; // Set to x to have UART TX on RPx
    int rxPin; // Set to x to have UART RX on RPx
    int baudrate; // Have a guess :) 
}uart_t;


void setup_uart(uart_t x);
void lock_config();
void unlock_config();

#endif	// UART_H 

