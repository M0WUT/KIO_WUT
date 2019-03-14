/* 
 * File: wk.c   
 * Author: Dan McGraw M0WUT
 */

#include "wk.h"

// Prototypes
static void clear_buffer(uart_t *x, uint16_t c);
static void handle_admin_command(uart_t *x);


uint16_t handle_wk(uart_t *x, char* response){
    // Make sure we have at least 1 character
    if((*x).charRcvd == 0)
        return 0;
    uint16_t responseLength = 0;
    // Winkey messages don't have any termination (boo!) so have to check the
    // the message EVERY SINGLE TIME
    // 'response' is what's sent to the K3
    // Returns number of chars from buffer to send to K3
    char i = 0;
    switch((*x).rxBuffer[0]){
        case 0x00: // Admin Command
            handle_admin_command(x); break;
        case 0x01: // Sidetone
            // Deliberate Fallthrough
        case 0x02: // Set WPM
             // Deliberate Fallthrough - This is controlled by K3 only
        case 0x03: // Set Weighting
            clear_buffer(x, 2);
            break;
        case 0x04: // Set PTT lead/tail
            clear_buffer(x, 3);
            break;
        case 0x05: // Setup Speed Pot
            clear_buffer(x, 4);
            break;
        case 0x06: // Set Pause State
            clear_buffer(x, 2);
            break;
        case 0x07: // Get Speed Pot
            i = 0x80; // Value indicating minimum speed
            add_to_tx_buffer(x, &i, 1);
            clear_buffer(x, 1);
            break;
        case 0x08: // Backspace
            clear_buffer(x, 2);
            break;
        case 0x09: // Pin Configurations
            clear_buffer(x, 2);
            break;
        case 0x0A: // Clear buffer
            response[0] = 'R';
            response[1] = 'X';
            response[2] = ';';
            responseLength = 3;
            clear_buffer(x, 1);
            break;
        case 0x0B: // Key immediate
            // Deliberate Fallthrough
        case 0x0C: // Set High Speed CW
            // Deliberate Fallthrough
        case 0x0D: // Set Farnsworth WPM
            // Deliberate Fallthrough
        case 0x0E: // Set Winkeyer Mode
            clear_buffer(x, 2);
            break;
        case 0x0F: // Load Defaults
            clear_buffer(x, 16);
            break;
        case 0x10: // Set 1st Extension
            // Deliberate Fallthrough
        case 0x11: // Set Key Comp
            // Deliberate Fallthrough
        case 0x12: // Set Paddle Switchpoint
            clear_buffer(x, 2);
            break;
        case 0x13: // NULL
            clear_buffer(x, 1);
            break;
        case 0x14: // Software Paddle
            clear_buffer(x, 2);
            break;
        case 0x15: // Status Request
            i = 0xC0; // Value indicating nothing happening
            add_to_tx_buffer(x, &i, 1);
            clear_buffer(x, 1);
            break;
        case 0x16: // Pointer Command
            // This is a bit fiddly, just going to hope nobody uses it!
            // Deliberate Fallthrough
        case 0x17: // Set Dit/Dah Ratio
            clear_buffer(x, 2);
            break;
        case 0x18: // PTT On/Off
            // Deliberate Fallthrough
        case 0x19: // Key Buffered
            // Deliberate Fallthrough
        case 0x1A: // Wait
            clear_buffer(x, 2);
            break;
        case 0x1B: // Merge 2 letters
            // Just no!
            clear_buffer(x, 1); // Remove modifier, allow both characters to be sent seperately
            break;
        case 0x1C: // Buffered Speed Change
            // Deliberate Fallthrough
        case 0x1D: // High Speed CW Speed Change
            clear_buffer(x, 2);
            break;
        case 0x1E: // Cancel buffered speed change
            // Deliberate Fallthrough
        case 0x1F: // Buffered NOP
            clear_buffer(x, 1);
            break;
        default: // Just a character to be sent
            add_to_tx_buffer(x, (*x).rxBuffer, 1);
            response[0] = 'K';
            response[1] = 'Y';
            response[2] = ' ';
            response[3] = (*x).rxBuffer[0];
            response[4] = ';';
            responseLength = 5;
            clear_buffer(x, 1);
            break;
    };
    return responseLength;
}


static void clear_buffer(uart_t *x, uint16_t c){
    // Wait until message is c characters long then wipe from buffer
    if((*x).charRcvd == c)
    {
        int i = 0;
        for(i = (*x).charRcvd; i < BUFFER_SIZE; i++)
            (*x).rxBuffer[i] = (*x).rxBuffer[i - (*x).charRcvd];
        (*x).charRcvd = 0;
    }
}


static void handle_admin_command(uart_t *x){
    // Ensure the 2nd character has been received
    if((*x).charRcvd == 0)
        return;
    char i = 0;
    switch((*x).rxBuffer[1]){
        case 0x00: // Calibrate
            clear_buffer(x, 3);
            break;
        case 0x01: // Reset
            clear_buffer(x, 2);
            break;
        case 0x02: // Host open
            i = 30; 
            add_to_tx_buffer(x, &i, 1); // Report Version Number
            clear_buffer(x, 2);
            break;
        case 0x03: // Host Close
            clear_buffer(x, 2);
            break;
        case 0x04: // Echo Test
            if ((*x).charRcvd == 3){
                add_to_tx_buffer(x, &((*x).rxBuffer[2]), 1);
            }
            clear_buffer(x, 3);
            break;
        case 0x05: // Paddle A/D
            // Deliberate Fallthrough
        case 0x06: // Speed A/D
            // Deliberate Fallthrough
        case 0x07: // Get Values
            i = 0;
            add_to_tx_buffer(x, &i, 1);
            clear_buffer(x, 2);
            break;
        case 0x08: // Debug Use Only
            clear_buffer(x, 2);
            break;
        case 0x09: // Get Cal
            i = 0;
            add_to_tx_buffer(x, &i, 1);
            clear_buffer(x, 2);
            break;
        case 0x0A: // Set WK1 Mode
            // Deliberate Fallthrough
        case 0x0B: // Set WK2 Mode
            clear_buffer(x, 2);
            break;
        case 0x0C: // Dump EEPROM
            // Deliberate Fallthrough
        case 0x0D: // Download EEPROM
            clear_buffer(x, 2);
            break;
        case 0x0E: // Standalone Message
            clear_buffer(x, 3);
            break;
        case 0x0F: // Load X1MODE
            // Deliberate Fallthrough
        case 0x10: // Firmware Update
            // Please don't!
            // Deliberate Fallthrough
        case 0x11: // Set High Baud
            // Deliberate Fallthrough
        case 0x12: // Set Low Baud
            // Deliberate Fallthrough
        case 0x13: // K1EL Antenna Switch
            // Deliberate Fallthrough
        case 0x14: // Set WK3 Mode
            clear_buffer(x, 2);
            break;
        case 0x15: // Read VCC
            i = 52; // Corresponds to ~5V
            add_to_tx_buffer(x, &i, 1);
            clear_buffer(x, 2);
            break;
        case 0x16: // Load X2MODE
            clear_buffer(x, 2);
            break;
        default: // Something is wrong, clear command or code will get stuck
            clear_buffer(x, 2);
            break;
    };
}