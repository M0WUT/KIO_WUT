/*
 * File:   uart.c
 * Author: Dan McGraw M0WUT
 *
 */

#include "stdint.h"
#include "uart.h"

// Address of first register of each UART's registers
uint16_t *const UART_BASE_ADDRESSES[] = {
    (uint16_t*) 0x0500,
    (uint16_t*) 0x0516,
    (uint16_t*) 0x052C,
    (uint16_t*) 0x0538,
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

static void unlock_config(){
    //I blindly copied these from datasheet
    asm volatile ("MOV #OSCCON, w1 \n"
        "MOV #0x46, w2 \n"
        "MOV #0x57, w3 \n"
        "MOV.b w2, [w1] \n"
        "MOV.b w3, [w1] \n"
        "BCLR OSCCON, #6"
    );
}

static void lock_config(){
    //I blindly copied these from datasheet
    asm volatile ("MOV #OSCCON, w1 \n"
        "MOV #0x46, w2 \n"
        "MOV #0x57, w3 \n"
        "MOV.b w2, [w1] \n"
        "MOV.b w3, [w1] \n"
        "BSET OSCCON, #6"
    ) ; 
}


void setup_uart(uart_t *x){

    (*x).charToTx = 0;
    (*x).charRcvd = 0;
    
    unlock_config();

    //Setup RX pin
    switch((*x).number)
    {
        //These are seemingly random in memory so have to be specific for each
        case 1: RPINR18bits.U1RXR = (*x).rxPin; break;
        case 2: RPINR19bits.U2RXR = (*x).rxPin; break;
        case 3: RPINR17bits.U3RXR = (*x).rxPin; break;
        case 4: RPINR27bits.U4RXR = (*x).rxPin; break;
        default: break;
    };

    // Disable all ADCs as it overrides UART RX
    ANSB = 0;
    
    //Setup TX Pin    
    //Function value for TX of each UART
    const uint16_t UART_TX_FUNCTION_VALUES[] = {3, 5, 19, 21};
    
    //Two pins per register, higher pin number in <13:8>, lower pin in <5:0>    
    uint16_t reg_offset = (*x).txPin >> 1; //divide by 2 to get offset from base 
    uint16_t high_low = (*x).txPin & 0x01; //shows high or low pin in reg
    
    //Clear the function bits for the pin used as TX
    *(OUTPUT_REG_BASE + reg_offset) &= (high_low ? 0xC0FF: 0xFFC0);
    //Set the function bits to the TX function for the chosen UART
    *(OUTPUT_REG_BASE + reg_offset) |= UART_TX_FUNCTION_VALUES[(*x).number - 1] << (high_low ? 8 : 0);
    
    lock_config();
    
    //Set TX pin as output
    TRISB &= ~(1 << (*x).txPin);
    //Ensure RX pin is input
    TRISB |= (1 << (*x).rxPin);
    
    //Base register for this UART (mostly to save typing!)
    (*x).breg = UART_BASE_ADDRESSES[(*x).number - 1];
    *((*x).breg + OFFSET_BRG) = 1000000 / (*x).baudrate - 1; // Eqn 18.2 in datasheet
    
    //Yes I know hard coded constants are bad but whatever...
    *((*x).breg + OFFSET_MODE) = (1<<15) | (1<<3) | ((*x).stopbits == 2) | (((*x).inverted & 0x01) << 4); // UART Enabled | High Baud Rate
    *((*x).breg + OFFSET_STATUS) = (1<<12) | (1<<10) | ((*x).inverted << 14); // RX Enable | Tx Enable
}

void add_to_tx_buffer(uart_t *x, char *a, uint16_t length){
    if ((*x).charToTx + length <= BUFFER_SIZE){
        uint16_t i = 0;
        for(i = 0; i < length; i++){
            (*x).txBuffer[(*x).charToTx++] = *(a+i);
        }
    }
    //If it doesn't fit in buffer, drop it
}

void handle_tx(uart_t *x){
    // While we have something to TX and the TX shift register is not full
    while(((*x).charToTx) && (((*((*x).breg + OFFSET_STATUS)) & (1<<9)) == 0)){
        *((*x).breg + OFFSET_TX) = (*x).txBuffer[0]; // Add char
        (*x).charToTx--; // One fewer char to TX
        uint16_t i = 1;
        //Shuffle FIFO down 1 bit
        for(i = 1; i < BUFFER_SIZE; i++)
            (*x).txBuffer[i - 1] = (*x).txBuffer[i];
    }
}

uint16_t handle_rx(uart_t *x){
    // All CAT Commands are terminated with a semicolon (;))
    // This loads received chars into RX buffer then returns buffer length
    // If no character received / not end of message, returns 0
    if(*((*x).breg + OFFSET_STATUS) & 0x01){
        //We have received something
        (*x).rxBuffer[(*x).charRcvd++] = *((*x).breg + OFFSET_RX);
        if((*x).rxBuffer[(*x).charRcvd - 1] == ';'){
            //Message complete
            return (*x).charRcvd;
        }
    }
    return 0;
}
