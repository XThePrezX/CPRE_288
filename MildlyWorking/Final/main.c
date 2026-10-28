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


int main (void) {

    timer_init();
    lcd_init();
    adc_init();
    uart_interrupt_init();
    servo_init();
    ping_init();
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    int i;
    servo_move(180);
    //    timer_waitMillis(500);
    servo_move(0);
    //    timer_waitMillis(500);
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

void move_backwards(oi_t *sensor_data){
    double sum = 0;
    //Move (15 cms = 150mm) back
    oi_setWheels(-100,-100) ;// at a slower speed, move back
    while(sum < 150){
        oi_update(sensor_data);
        sum -= sensor_data->distance; // use -> notation since pointer
    }
    oi_setWheels(0, 0); //stop
}

void move_forward(){
//    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
//    oi_init(sensor_data); // do this only once at start of main()
    oi_setWheels(150,150);
    int i;
    for (i = 0; i<1000000; i++){
//        oi_update(sensor_data);
//
//        if (sensor_data->cliffFrontLeftSignal < 20 ||  sensor_data->cliffFrontRightSignal < 20 ||  sensor_data->cliffLeftSignal < 20 || sensor_data->cliffRightSignal < 20) {
//            oi_setWheels(0, 0);
//
//            //If hole is on left side of the bot, turn right
//            if (sensor_data->cliffLeftSignal < sensor_data->cliffRightSignal) {
//                move_backwards(sensor_data);
//                turn_left();
//            }
//
//            //Otherwise, turn left
//            else {
//                move_backwards(sensor_data);
//                turn_right();
//            }
//        }
//        //If there is a boundary, move backwards and stay in the course
//        if (sensor_data->cliffFrontLeftSignal > 2700 || sensor_data->cliffFrontRightSignal > 2600) {
//
//            if (sensor_data->cliffLeftSignal < sensor_data->cliffRightSignal) {
//                move_backwards(sensor_data);
//                turn_left();
//            }
//            else {
//                move_backwards(sensor_data);
//                turn_right();
//            }
//        }
//
//        //If the bot bumps into a small object on the left, move around it to the right
//        if (sensor_data->bumpLeft) {
//            move_backwards(sensor_data);
//            oi_update(sensor_data);
//            turn_right();
//        }
//
//        //If the bot bumps into a small object on the right, move around it to the left
//        else if (sensor_data->bumpRight) {
//            move_backwards(sensor_data);
//            oi_update(sensor_data);
//            turn_left();
//        }


    }
    oi_setWheels(0,0);
}

