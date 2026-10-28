#include "lcd.h"
#include <math.h>
#include "servo.h"
#include "ping.h"
#include "adc.h"
#include "open_interface.h"
#include "Timer.h"
#include "uart.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
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

void move_backwards(oi_t *sensor_data);
void move_forward(oi_t *sensor_data, float distance_mm);
void turn_left(oi_t *sensor_data, double degrees);
void turn_right(oi_t *sensor_data, double degrees);

int scan(); // to scan objects

int main (void) {

    timer_init();
    lcd_init();
    adc_init();
    uart_init();
    uart_interrupt_init();
    servo_init();
    ping_init();


    char my_data;       // Variable to get bytes from Client
    char command[100];  // Buffer to store command from Client
    int index = 0;      // Index position within the command buffer

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    // Write to LCD so that we know the program is running
    lcd_printf("WATING FOR CONNECTION");

    while(1)
    {

        index = 0;  // Set index to the beginning of the command buffer
        my_data = uart_receive(); // Get first byte of the command from the Client

        // Get the rest of the command until a newline byte (i.e., '\n') received
        while(my_data != '\n' )
        {
            command[index] = my_data;  // Place byte into the command buffer
            index++;
            my_data = uart_receive(); // Get the next byte of the command
        }

        command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
        command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

        lcd_printf("Got: %s", command);  // Print received command to the LCD screen
//        cyBOT_Scan(0, &scanner);
        timer_waitMillis(500);

        if (command[0] == 'M') {
            //scan from 0 to 180 has been initiated
            lcd_printf("SCANNING ENVIRONMENT");

            scan();


//            int i;
//            for (i=0; i<181; i+=4) {
////                cyBOT_Scan(i, &scanner); //Set Scanner
//                //                uint32_t rawvalue = adc_read();
//                //                uint8_t irdistance = pow(((float)rawvalue)/13489.0, -1.5);
////                uint8_t send = scanner.sound_dist;
//                uart_sendChar(send);
//                uart_sendChar('\n');
//
//
//            }
        }


        // Send a response to the Client (Starter Client expects the response to end with \n)
        // In this case I am just sending back the first byte of the command received and a '\n'
        //        cyBot_sendByte(command[0]);

        // Only send a '\n' if the first byte of the command is not a '\n',
        // to avoid sending back-to-back '\n' to the client
        if(command[0] != '\n')
        {
            uart_sendChar('\n');
        }

    }

    return 0;
}

void turn_right(oi_t *sensor_data, double degrees){
    double sum = 0.0;
    oi_setWheels(-50,50); // move forward speed
    while(abs(sum) < degrees){
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }
    oi_setWheels(0,0); //stop
}

void turn_left(oi_t *sensor_data, double degrees){
    double sum = 0.0;
    oi_setWheels(50,-50);
    while(abs(sum) < degrees){
        oi_update(sensor_data);
        sum += sensor_data->angle;
    }
    oi_setWheels(0,0); //stop
}

void move_forward(oi_t *sensor_data, float distance_mm){
    double sum = 0.0;
    oi_setWheels(150,150); //Move forward at full speed

    //Move required distance
    while(sum < distance_mm){
        oi_update(sensor_data);
        sum += sensor_data->distance;
    }
    oi_setWheels(0,0); //stop
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


int scan(){
    int i;
    int objectTypes = 0; //0 is no object, 1 is single object, 2 is a group of objects
    int objectCounts = 0;
    char python[100];
    //Values for displaying data
    char start[] = "Degrees\t\tDistance (cm)\t\tIRValue\t\tDirection\n\r";
    char end[] = "Object#\t\tAngle\t\tPing Distance\tADC Distance\tWidth\tType\n\r";
    char left = 'l';
    char right = 'r';
    char direction;

//    uart_sendStr("\n\r");
//    uart_sendStr(start);

    //Values to store data collected from the scan
    char layout[100];
    short objectCount = 0;
    //objects objectArr[10];

    //Values to note whether and object is being scanned
    int previousVal = 0;
    int previousAngle = 0;
    int curVal;
    int distVal = 70;
    int Ir_Val ;

    //Scan 180 degrees
    for (i = 0; i <= 180; i += 4 ) {  //2
        servo_move(i);

        //Give bot time to turn
        if (i == 0) {
            timer_waitMillis(500);
        }

        if(i < 90){
            direction = 'R' ;

        }
        else{
            direction = 'L' ;
        }
        //Store current IR distance in cm
        curVal = 654371 * pow(adc_read(), -1.420); // can be changed later
        uart_sendChar((uint8_t)curVal);
        uart_sendChar('\n');
        Ir_Val = adc_read();

        //If an object is scanned, store its values for faster scanning
//        if (curVal < distVal && previousVal > distVal) {
//            objectArr[objectCount].object = objectCount + 1;
//            objectArr[objectCount].start_angle = i;
//            objectArr[objectCount].adc_distance = curVal;
//            objectCount++;
//        }
//
//        //If object is still being scanned, continue to update width
//        else if (previousVal < distVal) {
//            objectArr[objectCount - 1].end_angle = previousAngle;
//        }

        sprintf(layout, "%d\t\t%d\t\t%d\t\t\t%c\n\r", i, curVal,Ir_Val,direction);

//        uart_sendStr(layout);

        previousVal = curVal;
        previousAngle = i;
    }
//    uart_sendStr("\n\r");

//    for (i = 0; i < objectCount; i++) {
//        sprintf(layout, "%d\t\t%d\t\t%0.2f\t\t%d\t\t%0.2f\t%c\n\r", objectArr[i].object, objectArr[i].mid_angle, objectArr[i].ping_distance, objectArr[i].adc_distance, objectArr[i].linear_width, objectArr[i].type);
//        uart_sendStr(layout);
//    }
//
//    for (i = 0; i < objectCount; i++) {
//        sprintf(python,"\n!%d!%0.2f!%c\n\r", objectArr[i].mid_angle, objectArr[i].ping_distance, objectArr[i].type);
//        uart_sendStr(python);
//    }

    servo_move(0);

    return objectTypes;
}

