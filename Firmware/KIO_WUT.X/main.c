/*
 * File:   main.c
 * Author: Dan McGraw M0WUT
 *
 */
#pragma config POSCMD = HS
#pragma config FNOSC = FRC
#pragma config FWDTEN = OFF
#pragma config JTAGEN = OFF
#pragma config ICS = PGx2
#pragma config IOL1WAY = OFF

#include "uart.h"

int main(void) {
       
    uart_t CAT = (uart_t){.number=1, .txPin =8, .rxPin=9, .baudrate=38400, .stopbits=1};
    uart_t WK = (uart_t){.number=2, .txPin =14, .rxPin=15, .baudrate=1200, .stopbits=2};
    uart_t K3 = (uart_t){.number=3, .txPin =1, .rxPin=0, .baudrate=38400, .stopbits=1};
    uart_t P3 = (uart_t){.number=4, .txPin =3, .rxPin=2, .baudrate=38400, .stopbits=1};

    setup_uart(CAT);
    setup_uart(WK);
    setup_uart(K3);
    setup_uart(P3);
 
    while(1){
        U1TXREG = '1';
        U2TXREG = '2';
        U3TXREG = '3';
        U4TXREG = '4';
        int i = 0;
        while(++i < 0xFFFF);
    }
}
