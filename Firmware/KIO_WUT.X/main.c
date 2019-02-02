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
#include "wk.h"

char catRxBuffer[10];
char catTxBuffer[50];

char wkRxBuffer[10];
char wkTxBuffer[50];

char k3RxBuffer[10];
char k3TxBuffer[50];

char p3RxBuffer[10];
char p3TxBuffer[50];

char k3WkBuffer[4];
uint16_t wk_bytesToRx = 0;


int main(void) {
       
    uart_t CAT = (uart_t){.number=1, .txPin =8, .rxPin=9, .baudrate=38400, .stopbits=1};
    uart_t WK = (uart_t){.number=2, .txPin =14, .rxPin=15, .baudrate=1200, .stopbits=2};
    uart_t K3 = (uart_t){.number=3, .txPin =1, .rxPin=0, .baudrate=38400, .stopbits=1};
    uart_t P3 = (uart_t){.number=4, .txPin =3, .rxPin=2, .baudrate=38400, .stopbits=1};

    setup_uart(&CAT);
    setup_uart(&WK);
    setup_uart(&K3);
    setup_uart(&P3);
 
    uint16_t x;
    uint16_t wk_charRcvd = 0;

    while(1){
      
        // Ignoring the Winkey, messages between the other 3 ports are simply forwarded
        // This pattern obeys the following:
        // IN           OUT
        // CAT          K3
        // P3           K3
        // K3           CAT, P3
        // Messages to the Winkey are either automatically responded to (which 
        // may include ignoring them) or forwarded on only to the K3 after 
        //converting to CAT commands
        
        x = handle_rx(&CAT); // x contains length of message from CAT
        if(x)
        {
            add_to_tx_buffer(&K3, CAT.rxBuffer, x);          
            CAT.charRcvd = 0;
        }
        
        x = handle_rx(&P3);
        if(x)
        {
            add_to_tx_buffer(&K3, P3.rxBuffer, x);          
            P3.charRcvd = 0;
        }
        
        x = handle_rx(&K3);
        if(x)
        {
            add_to_tx_buffer(&CAT, K3.rxBuffer, x);          
            add_to_tx_buffer(&P3, K3.rxBuffer, x);          
            K3.charRcvd = 0;
        }
        
        // WK is not ; terminated so still use handle_rx to process but always check the message
        handle_rx(&WK);
        if(WK.charRcvd != wk_charRcvd){ // Another char received since last time
            uint16_t responseLength = handle_wk(&WK, k3WkBuffer);
            if(responseLength)
                add_to_tx_buffer(&K3, k3WkBuffer, responseLength);
            
            wk_charRcvd = WK.charRcvd;
        }
        
        handle_tx(&CAT);
        handle_tx(&WK);
        handle_tx(&K3);
        handle_tx(&P3);
    }
}
