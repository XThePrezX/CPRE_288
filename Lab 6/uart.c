/*
 *
 *   uart.c
 *
 *
 *
 *   @author
 *   @date
 */

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"

void uart_init(void){
    //TODO
    //enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //enable clock to UART1
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

    //wait for GPIOB and UART1 peripherals to be ready
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {};
    while ((SYSCTL_PRUART_R & 0x20) == 0) {};

    //enable alternate functions on port B pins
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);

    //enable digital functionality on port B pins
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1);

    //enable UART1 Rx and Tx on port B pins
    GPIO_PORTB_PCTL_R = 0x00000011;

    //calculate baud rate
    uint16_t iBRD = 8; //use equations
    uint16_t fBRD = 44; //use equations

    //turn off UART1 while setting it up
    UART1_CTL_R &= ~(UART_CTL_UARTEN);

    //set baud rate
    //note: to take effect, there must be a write to LCRH after these assignments
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
    //note: this write to LCRH must be after the BRD assignments
    UART1_LCRH_R = UART_LCRH_WLEN_8;

    //use system clock as source
    //note from the datasheet UARTCCC register description:
    //field is 0 (system clock) by default on reset
    //Good to be explicit in your code
    UART1_CC_R = UART1_CC_R;

    //re-enable UART1 and also enable RX, TX (three bits)
    //note from the datasheet UARTCTL register description:
    //RX and TX are enabled by default on reset
    //Good to be explicit in your code
    //Be careful to not clear RX and TX enable bits
    //(either preserve if already set or set them)
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

}

void uart_sendChar(char data){
    ///wait until there is room to send data
    while (UART1_FR_R & 0x20)
    {
    }
    ///send data
    UART1_DR_R = data;
}

char uart_receive(void){
    //TODO
}

void uart_sendStr(const char *data){
    //TODO for reference see lcd_puts from lcd.c file
}
