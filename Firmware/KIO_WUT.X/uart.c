/*
 * File:   uart.c
 * Author: Dan McGraw M0WUT
 *
 */


#include "stdint.h"
#include "uart.h"


//Making UART library scalable across devices, likely I'll use it later

// Address of first register of each UART's registers
uint16_t *const const UART_BASE_ADDRESSES[] = { 
    (uint16_t*)0x0500,
    (uint16_t*)0x0516,
    (uint16_t*)0x052C,
    (uint16_t*)0x0538
};

// Offset for each UART register from base address
#define OFFSET_MODE 0x00
#define OFFSET_STATUS 0x02
#define OFFSET_TX 0x04
#define OFFSET_RX 0x06
#define OFFSET_BRG 0x08
#define OFFSET_ADMD 0x0A
#define OFFSET_SCCON 0x0C
#define OFFSET_SCINT 0x0E
#define OFFSET_GTC 0x10
#define OFFSET_WTCL 0x12
#define OFFSET_WTCH 0x14

int foo(uart_t x){
    *(UART_BASE_ADDRESSES[x.number] + OFFSET_TX) += 1;
    return 1;
}




