/*
 * servo.c
 *
 *  Created on: Apr 12, 2024
 *      Author: pgask26
 */

#include "servo.h"
#include "stdint.h"
#include "ping_template.h"
#include <stdbool.h>
#include "Timer.h"
#include "lcd.h"
#include "driverlib/interrupt.h"

//Initalize the servo
//initializing/configuring the PWM timer
void servo_init(void) {

    SYSCTL_RCGCGPIO_R |= 0b10; //GPIO port b
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};

    SYSCTL_RCGCTIMER_R |= 0b10; //Timer module 1
    while ((SYSCTL_PRTIMER_R & 0b10) == 0) {};

    GPIO_PORTB_AFSEL_R |= 0x20; // disable alternate function for pin 5 because using it for output
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF0FFFFF) | 0x00700000;//PRELAB // choosing the alternate function to be timer 3b CCP
    GPIO_PORTB_DEN_R |= 0x20; // enabling pin 5
    GPIO_PORTB_DIR_R |= 0x20; // setting pin 5 as output


    //initializing/configuring the PWM timer

    //1. Disable the selected timer by clearing the TnEN bit in the GPTMCTL register.
    TIMER1_CTL_R &= ~0x100;
    //2. Initialize the GPTMCFG register by writing 0x4 to setup all timers as 16-bit default timers.
    TIMER1_CFG_R |= 0x4;
    /*3. Configure the TnMR, TnAMS and TnCMR fields in the GPTMTnMR register by writing:
        ¢ TnMR = 0x2 for periodic timer mode.
        ¢ TnCMR = 0x0 for edge count mode.
        ¢ TnAMS = 0x1 for PWM mode*/
    TIMER1_TBMR_R |= 0xA;
    TIMER1_TBMR_R &= ~0x4;

    //4. Configure the output state of the PWM signal, either inverted or not, in the TnPWML field of the GPTMCTL register.
    TIMER1_CTL_R |= 0x4000;
    //5. If a prescaler is to be used, write the prescale value to the GPTMTnPR register
    TIMER1_TBPR_R = 0xFF; //Use prescaler extension to 24 bits
    TIMER1_TBILR_R = 0xFFFF; //Do we need this line?
    //7. Load the timer start value into the GPTMTnILR register.
//    TIMER1_TBILR_R = ;
    //8. Load the GPTMTnMATCHR register with the match value.
    TIMER1_TBMATCHR_R = ;
    //9. Set the TnEN bit in the GPTMCTL register to enable the timer and begin generation of the output PWM signal.
    TIMER1_CTL_R |= 0x100;
}

//positioning the servo
void servo_move(uint16_t degrees) {

    TIMER1_TBILR_R
    TIMER1_TBPR_R
    TIMER1_TBMATCHR_R

}
