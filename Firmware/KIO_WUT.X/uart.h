/* 
 * File: uart.h   
 * Author: Dan McGraw M0WUT
 */
 
#ifndef UART_H
#define	UART_H



// slightly unnecessary struct but means I can name UARTs rather
// than remember which number they are
typedef struct uart{
    int number; // Which number UART this is  
    int txPin; // Set to x to have UART TX on RPx
    int rxPin; // Set to x to have UART RX on RPx
    int baudrate; // Have a guess :) 
}uart_t;

int foo(uart_t x);

#endif	// UART_H 

