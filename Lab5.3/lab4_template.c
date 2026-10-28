/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include <uart.h>
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
// PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#define REPLACEME 0



int main(void) {
    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    // Don't forget to initialize the cyBot UART before trying to use it
    uart_init();

    /*
    cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code
    SYSCTL_RCGCGPIO_R |= 0b00000010;
    while ((SYSCTL_PRGPIO_R & 0b00000010) == 0) {}; //wait for system control timer
    GPIO_PORTB_AFSEL_R |= 0x00000003; //enable alternate functions for PB pins
    GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations
    GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations
    GPIO_PORTB_DEN_R |= 0x00000003; */

    cyBot_uart_init_last_half();  // Complete the UART device configuration

    // YOUR CODE HERE
    uint8_t number = 0;

    while(1)
    {
        char data = uart_receive();
        uart_sendChar(data);
    }


}
