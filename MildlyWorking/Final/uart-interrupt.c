/*
 *
 *   uart-interrupt.c
 *
 *
 *
 *   @author
 *   @date
 */

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include "uart-interrupt.h"
#include <lcd.h>
#include "lcd.h"
#include "Timer.h"
#include <stdio.h>
#include <stdbool.h>
#include <main.h>
#include <servo.h>
#include <adc.h>

char command[100];

void scan(void);

void uart_interrupt_init(void){
    SYSCTL_RCGCGPIO_R |= 0b10;
    SYSCTL_RCGCUART_R |= 0b10;
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};
    while ((SYSCTL_PRUART_R & 0b10) == 0) {};
    GPIO_PORTB_DEN_R |= 0x3;
    GPIO_PORTB_AFSEL_R |= 0x3;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFF00) + 0x00000011;
    uint16_t iBRD = (int)16000000/(116000*16); //use equations
    uint16_t fBRD = 44; //use equations
    UART1_CTL_R &= 0xFFFFFFFE;
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;
    UART1_LCRH_R = 0x60;
    UART1_CC_R = 0x0;
    UART1_ICR_R |= 0b00010000;
    UART1_IM_R |= 0x10;
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;
    NVIC_EN0_R |= 0b01000000;
    IntRegister(INT_UART1, UART1_Handler);
    IntMasterEnable();
    UART1_CTL_R |= 0b1100000001;
}

void uart_sendChar(char data){
    while (UART1_FR_R & 0x20) {}
    UART1_DR_R = data;
}

char uart_receive(void){
    char data = 0;
    while(UART1_FR_R & UART_FR_RXFE)
    {
    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);
    return data;
}

void uart_sendStr(const char *data){
    //TODO for reference see lcd_puts from lcd.c file
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char my_data;       // Variable to get bytes from Client
    char command[100];  // Buffer to store command from Client
    int index = 0;      // Index position within the command buffer

    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10)
    {
        index = 0;  // Set index to the beginning of the command buffer
        my_data = (char)(UART1_DR_R & 0xFF);

        // Get the rest of the command until a newline byte (i.e., '\n') received
        while(my_data != '\n' )
        {
            command[index] = my_data;  // Place byte into the command buffer
            index++;
            my_data = uart_receive(); // Get the next byte of the command
        }
        UART1_ICR_R |= 0b00010000;

        command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
        command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

        lcd_printf("Got: %s", command);  // Print received command to the LCD screen

        if(command[0] != '\n')
        {
            uart_sendChar('\n');
        }

        if (command[0] == 'M') {
            scan();
        }

        if (command[0] == 'F') {
            move_forward();
        }
        if (command[0] == 'A') {
            turn_left();
        }
        if (command[0] == 'D') {
            turn_right();
        }
    }

}

void scan(){
    int i;
    int curVal;
    lcd_printf("SCANNING");

    //Scan 180 degrees
    for (i = 0; i <= 180; i += 4 ) {  //2
        servo_move(i);
        curVal = 654371 * pow(adc_read(), -1.420); // can be changed later
        uart_sendChar((uint8_t)curVal);
        uart_sendChar('\n');
    }
    servo_move(0);
    return;
}
