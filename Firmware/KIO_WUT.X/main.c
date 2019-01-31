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

#define LOOPBACK

int main(void) {
       
    uart_t CAT = (uart_t){.number=1, .txPin =8, .rxPin=9, .baudrate=38400, .stopbits=1};
    uart_t WK = (uart_t){.number=2, .txPin =14, .rxPin=15, .baudrate=1200, .stopbits=2};
    uart_t K3 = (uart_t){.number=3, .txPin =1, .rxPin=0, .baudrate=38400, .stopbits=1};
    uart_t P3 = (uart_t){.number=4, .txPin =3, .rxPin=2, .baudrate=38400, .stopbits=1};

    setup_uart(CAT);
    setup_uart(WK);
    setup_uart(K3);
    setup_uart(P3);
 

#ifdef LOOPBACK
    char x;
    while(1){
        

        x = rx_char(CAT);
        if(x)
            tx_char(CAT, x);
        
        x = rx_char(WK);
        if(x)
            tx_char(WK, x);
        
        x = rx_char(K3);
        if(x)
            tx_char(K3, x);
        
        x = rx_char(P3);
        if(x)
            tx_char(P3, x);


    }
#endif // ifdef LOOPBACK
}


