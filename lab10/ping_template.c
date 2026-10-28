/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
volatile bool overflow = false;

void ping_init (void){
    //RCGCGPIO
    //RCGCTIMER
    //while PRTIMER
    //while PRGPIO
    SYSCTL_RCGCGPIO_R |= 0b10; //GPIO port b
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};

    SYSCTL_RCGCTIMER_R |= 0b1000; //Timer module 3
    while ((SYSCTL_PRTIMER_R & 0b1000) == 0) {};

    // YOUR CODE HERE
    GPIO_PORTB_AFSEL_R |= 0x8; // disable alternate function for pin 3 because using it for output
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0FFF) | 0x00007000;//PRELAB // choosing the alternate function to be timer 3b CCP
    GPIO_PORTB_DEN_R |= 0x8; // enabling pin 3
    GPIO_PORTB_DIR_R |= 0x8; // setting pin 3 as output

    // Configure and enable the timer

    TIMER3_CTL_R &= ~0x100; // disable timer to configure
    TIMER3_CFG_R |= 0x4; // splits timer3 into two 16 bit timers, we will use timer b and a prescalar
    TIMER3_TBMR_R |= 0x7; // set timer 3b to capture mode and edge time mode
    TIMER3_TBMR_R &= ~0x10; // sets count direction to down, check what to set (TAAMS)

    TIMER3_CTL_R |= 0xC00; // setting bits 11, 10 (trigger interrupt for both edges)
    TIMER3_TBILR_R |= 0xFFFF; // sets value of 16 bit timer to full
    TIMER3_TBPR_R |= 0xFF; // sets value of 8 bit pre-scalar to full
    TIMER3_IMR_R |= 0x400; // set mask

    TIMER3_ICR_R = 0x400; // Clear an interrupt that may have been erroneously triggered

    TIMER3_CTL_R |= 0x100; // enable time 3b

    NVIC_EN1_R |= 0x10; // NVIC enable for 52

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}

void ping_trigger (void){
    STATE = LOW;
    overflow = false;

    TIMER3_CTL_R &= ~0x100; //disable timer
    TIMER3_IMR_R &= ~0x400; //disable timer interrupt
    GPIO_PORTB_AFSEL_R &= ~0x8; // disable afsel

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    // set the PB3 pin 3 data to 1 to start pulse
    //something GPIO port B

    GPIO_PORTB_DATA_R &= ~0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5); //2 microsecond pulse, can be 2-5
    GPIO_PORTB_DATA_R &= ~0x8;

    TIMER3_ICR_R = 0x400;// Clear an interrupt that may have been erroneously triggered

    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_IMR_R |= 0x400; // Re-enable timer interrupt
    TIMER3_CTL_R |= 0xD00; // Re-enable timer
}

void TIMER3B_Handler(void){

    // YOUR CODE HERE
    // As needed, go back to review your interrupt handler code for the UART lab.
    // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
    // includes checking the source of the interrupt and clearing the interrupt status bit.
    // Checking the source: test the MIS bit in the MIS register (is the ISR executing
    // because the input capture event happened and interrupts were enabled for that event?
    // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
    // The rest of the code in the ISR depends on actions needed when the event happens.

    // interrupt means that either the rising or falling edge has been detected, use STATE variable
    // to determine which it is
    if (TIMER3_MIS_R & 0x400) {
        TIMER3_ICR_R = 0x400;

        if (STATE == LOW) {
            //detecting the rising edge of echo
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;

        } else if (STATE == HIGH){
            //detecting the falling edge of the echo
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        }
    }
}

float ping_getDistance (void){

    // YOUR CODE HERE
    //speed of sound is 343m/s, sound needs to do a round trip to get back to the sonar sensor
    ping_trigger();

    while(STATE!=DONE) {}


    float bitDifference = (float)START_TIME - END_TIME;
    float timeDifference = bitDifference * 0.000000625;
    float cmDistance = timeDifference * 1715;


    if(cmDistance<0) {
        overflow = true;
        return 0;
    }


    return cmDistance;
}
