/* 
 * File: uart.h   
 * Author: Dan McGraw M0WUT
 */
 
#ifndef UART_H
#define	UART_H

#include <xc.h>

#define BUFFER_SIZE 50

typedef struct uart{
    uint16_t number; // Which number UART this is  
    uint16_t txPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint16_t rxPin; // pin on port B to use as TX (N.B. only those that support PPS)
    uint32_t baudrate; // Have a guess :) 
    uint16_t stopbits;
    uint16_t *breg; // Base address for UART's config registers, just to save recalculating every time
    char rxBuffer[BUFFER_SIZE];
    char txBuffer[BUFFER_SIZE];
    uint16_t charToTx;
    uint16_t charRcvd;
    
} uart_t;


void setup_uart(uart_t *x);
void add_to_tx_buffer(uart_t *x, char *a, uint16_t length); // Adds string to TX FIFO
void handle_tx (uart_t *x);
uint16_t handle_rx (uart_t *x);

#endif	// UART_H 


