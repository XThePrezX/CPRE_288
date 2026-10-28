/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Mearged Spring 2019 version with Fall 2018
 */




#include "button.h"
#include "lcd.h"
// Global varibles
//volatile int button_event;
volatile int button_num;

/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
    static uint8_t initialized = 0;
    lcd_init();
        //Check if already initialized
        if(initialized){
            return;
        }

        SYSCTL_RCGCGPIO_R |= 0b010000;
        GPIO_PORTE_DIR_R &= 0b00000000;
        GPIO_PORTE_DEN_R |= 0b11111111;


        initialized = 1;
}



///**
// * Initialize and configure PORTE interupts
// */
//void init_button_interrupts() {
//
//    // 1) Mask the bits for pins 0-3
//    GPIO_PORTE_IM_R &= 0b11110000;
//
//    // 2) Set pins 0-3 to use edge sensing
//    GPIO_PORTE_IS_R &= 0xF0;
//
//    // 3) Set pins 0-3 to use both edges. We want to update the LCD
//    //    when a button is pressed, and when the button is released.
//    GPIO_PORTE_IBE_R |=0x0F;
//
//    // 4) Clear the interrupt s
//    GPIO_PORTE_ICR_R = 0xFF;
//
//    // 5) Unmask the bits for pins 0-3
//    GPIO_PORTE_IM_R |= 0x0F;
//
//    // 6) Enable GPIO port E interrupt
//    NVIC_EN0_R |= 0b10000;
//
//    // Bind the interrupt to the handler.
//    IntRegister(INT_GPIOE, gpioe_handler);
//}
//
//
///**
// * Interrupt handler -- executes when a GPIO PortE hardware event occurs (i.e., for this lab a button is pressed)
// */
//void gpioe_handler() {
//
//    // Clear interrupt status register
//    GPIO_PORTE_ICR_R = 0xFF;
//    button_event = 1;
//    button_num = button_getButton();
//}



int button_getButton() {

    if((GPIO_PORTE_DATA_R & 0b00001000) == 0b00000000){
        return 4; // rightmost button
    }
    else if((GPIO_PORTE_DATA_R & 0b00000100) == 0b00000000){
        return 3;
    }
    else if((GPIO_PORTE_DATA_R & 0b00000010) == 0b00000000){
        return 2;
    }
    else if ((GPIO_PORTE_DATA_R & 0b00000001) == 0b00000000){
        return 1; //leftmost button
    }
    return 0;
}





