/*
 * File:   main.c
 * Author: Dan McGraw M0WUT
 *
 */
#pragma config POSCMD = HS
#pragma config FNOSC = FRC
#pragma config FWDTEN = OFF

#define XTAL_FREQUENCY 4000000

#include "uart.h"

int main(void) {
    
    
    uart_t CAT;
    CAT.number = 1; 
    CAT.txPin = 14;
    CAT.rxPin = 15;
    CAT.baudrate = 38400;
    TRISBbits.TRISB14 = 0;
    


    setup_uart(CAT);
   


    
    
    
    while(1){
        while (U1STAbits.UTXBF == 1);
        U1TXREG = 0x55;

    }
    
}
