#include "lcd.h"
#include <math.h>
#include "servo.h"
#include "ping.h"
#include "adc.h"
#include "open_interface.h"
#include "Timer.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <uart-interrupt.h>
#include <inc/tm4c123gh6pm.h>
#include "music.h"

extern volatile enum {LOW, HIGH, DONE} state;  //Set by ISR // in lab9 ping.c or ping_templace.c ours is ping_template.c
extern volatile unsigned int rising_time;  //Pulse start time  // in lab9 ping.c or ping_templace.c
extern volatile unsigned int falling_time; //Pulse end time  // in lab9 ping.c or ping_templace.c
extern volatile char uart_data;
extern volatile char flag;
extern volatile int degrees;
extern volatile int direction;
extern volatile float pcv;
extern volatile int right_calibration_val = 313200; // set
extern volatile int left_calibration_val = 287172;  // set

void move_forward();
void turn_left();
void turn_right();
int scan();

int main (void) {

    timer_init();
    lcd_init();
    adc_init();
    uart_interrupt_init();
    servo_init();
    ping_init();
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    // Write to LCD so that we know the program is running
    lcd_printf("WATING FOR CONNECTION");

    while(1)
    {

    }

    return 0;
}

void turn_right(){
    oi_setWheels(-50,50);
    int i;
    for (i = 0; i<1000000; i++){}
    oi_setWheels(0,0);
}

void turn_left(){
    oi_setWheels(50,-50);
    int i;
    for (i = 0; i<1000000; i++){}
    oi_setWheels(0,0);
}

void move_forward(){
    oi_setWheels(150,150);
    int i;
    for (i = 0; i<1000000; i++){}
    oi_setWheels(0,0);
}


int scan(){
    int i;
    int curVal;
    uart_sendChar('j');
    uart_sendChar('\n');
    for (i = 0; i <= 180; i += 4 ) {
        servo_move(i);
        for (i = 0; i<1000000; i++){}
        curVal = pow(adc_read()/13489.0, -1.5);
        printf("%d", curVal); // 4 debug
        lcd_printf("%d", (uint8_t)curVal);
        uart_sendChar((uint8_t)curVal);
        uart_sendChar('\n');
    }
    uart_sendChar('k');
    uart_sendChar('\n');
    servo_move(0);
    return 0;
}

