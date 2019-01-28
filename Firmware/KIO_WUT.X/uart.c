/*
 * File:   uart.c
 * Author: Dan McGraw M0WUT
 *
 */


#include "stdint.h"
#include "uart.h"


//Making UART library scalable across devices, likely I'll use it later

// Address of first register of each UART's registers
uint16_t *const UART_BASE_ADDRESSES[] = { 
    (uint16_t*)0x0500,
    (uint16_t*)0x0516,
    (uint16_t*)0x052C,
    (uint16_t*)0x0538
};

uint16_t *const OUTPUT_REG_BASE = (uint16_t*)0x03D6;

// Offset for each UART register from base address
#define OFFSET_MODE 0x00
#define OFFSET_STATUS 0x01
#define OFFSET_TX 0x02
#define OFFSET_RX 0x03
#define OFFSET_BRG 0x04

//Don't think the following are needed
/*
#define OFFSET_ADMD 0x05
#define OFFSET_SCCON 0x06
#define OFFSET_SCINT 0x07
#define OFFSET_GTC 0x08
#define OFFSET_WTCL 0x09
#define OFFSET_WTCH 0x0A
*/

void unlock_config(){
    //I blindly copied these from datasheet
    asm volatile ("MOV #OSCCON, w1 \n"
        "MOV #0x46, w2 \n"
        "MOV #0x57, w3 \n"
        "MOV.b w2, [w1] \n"
        "MOV.b w3, [w1] \n"
        "BCLR OSCCON, #6"
    );
}

void lock_config(){
    //I blindly copied these from datasheet
    asm volatile ("MOV #OSCCON, w1 \n"
        "MOV #0x46, w2 \n"
        "MOV #0x57, w3 \n"
        "MOV.b w2, [w1] \n"
        "MOV.b w3, [w1] \n"
        "BSET OSCCON, #6"
    ) ; 
}



void setup_uart(uart_t x){
    
    unlock_config();
   
    /*
    //Setup RX pin
    switch(x.number)
    {
        //These are seemingly random in memory so have to be specific for each
        case 1: RPINR18bits.U1RXR = x.rxPin; break;
        case 2: RPINR19bits.U2RXR = x.rxPin; break;
        case 3: RPINR17bits.U3RXR = x.rxPin; break;
        case 4: RPINR27bits.U4RXR = x.rxPin; break;
        default: break;
    };
    */
    

    
    //Setup TX pin
    //Two pins per register, higher pin number in <13:8>, lower pin in <5:0>
    
    int reg_offset = x.txPin >> 1; //clear LSB to give byte offset from base 
    int high_low = x.txPin & 0x01; //adjusting higher or low pin number in reg?
    
    //Function value for TX of each UART
    const uint16_t UART_TX_FUNCTION_VALUES[] = {3, 5, 19, 21};
    
    
    //Clear the function bits for the pin used as TX
    //RPOR7 &= (high_low ? 0xC0FF : 0xFFC0);
    //Set the function bits to the TX function for the chosen UART
    *(OUTPUT_REG_BASE + reg_offset) &= (high_low ? 0xC0FF: 0xFFC0);
    *(OUTPUT_REG_BASE + reg_offset) |= UART_TX_FUNCTION_VALUES[x.number - 1] << (high_low ? 8 : 0);
    
    lock_config();
    
    

    
    //Yes I know hard coded constants are bad but whatever...
    U1MODEbits.STSEL = 0; // 1-stop bit
    U1MODEbits.PDSEL = 0; // No parity, 8-data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disables
    U1MODEbits.BRGH = 1; // high speed mode
    U1BRG = 25;

    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1; // enable UART TX
    U1STAbits.URXEN = 1; // enable UART TX
    
 

}




