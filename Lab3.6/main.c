/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include <string.h>
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include "custom.h"
#include "cyBot_Scan.h"
#include <detectedObject.h>


void main (void) {

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    cyBot_uart_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    //PART1 CYBOT COMMUNICATION USING PUTTY
    cyBOT_Scan_t scanner;
    calloc(1, sizeof(cyBOT_Scan_t));

    //PART2 COLLECT SENSOR DATA AND IDENTIFY OBJECTS IN THE DATA
    //TWO OR MORE TALL OBJECTS SPACED APART IN FIELD
    cyBOT_init_Scan(0b0111);

    float threeFloats[3];
    float averageOfThree;
    int startingDegree=0, endingDegree=0;


    //custom struct for detected objects
    struct detectedObject object1, object2;

    while (1) {
        int i;
        bool detectingObject = false;
        for (i = 0; i<181; i+=2) { //scans every 2 degrees from 0 to 180 degrees (inclusive)
            cyBOT_Scan(i, &scanner); //Set Scanner

            int avgIndex = (i%6)/2;
            threeFloats[avgIndex] = scanner.sound_dist;

            if (avgIndex == 2) { //find the average of the three floats stored in threeFloats
                float sum = 0;
                int j;
                for (j=0; j<3; j++) {
                    sum += threeFloats[j];
                }
                float averageOfThree = sum/3.0; //averageOfThree is now effective sensor distance & i-2 is degree of sensor

                //Prints sensor values to Putty
                char strDistance[20] = {0};
                float distance = averageOfThree;
                sprintf(strDistance, "%f", distance);
                char strDegree[20] = {0};
                int u = i-2;
                sprintf(strDegree, "%d", u);
                PuttyString(strDistance);
                PuttyString("cm @ ");
                PuttyString(strDegree);
                PuttyString(" degrees\n\r");

                if (averageOfThree < 40) {//object has been detected
                    if (detectingObject == true) { //already started scanning an object
                    }
                    else { //just started scanning an object
                        detectingObject = true;
                        startingDegree = i-2;
                    }


                } else {
                    if (detectingObject == true) { //can no longer see detected object
                        detectingObject = false;
                        endingDegree = i-2;
                        char strRangeOfObject[20] = {0};
                        int rangeOfObject = endingDegree-startingDegree;
                        sprintf(strRangeOfObject, "%d", rangeOfObject);
                        PuttyString(strRangeOfObject);
                        PuttyString(" degree wide object detected!!\n\r");

                        if (object1.midpoint) { //object 1 has already been found
                            object1.objectSize = rangeOfObject;
                            object1.midpoint = (startingDegree+endingDegree)/2;
                            object1.distanceToObject = averageOfThree; //only the distance to the last angle

                        }else { //object 1 has not been found
                            object2.objectSize = rangeOfObject;
                            object2.midpoint = (startingDegree+endingDegree)/2;
                            object2.distanceToObject = averageOfThree; //only the distance to the last angle
                        }
                    }

                }


            }

            if (i==180) {
                if (detectingObject == true) { //can no longer see detected object
                    detectingObject = false;
                    endingDegree = i-2;
                    char strRangeOfObject[20] = {0};
                    int rangeOfObject = endingDegree-startingDegree;
                    sprintf(strRangeOfObject, "%d", rangeOfObject);
                    PuttyString(strRangeOfObject);
                    PuttyString(" degree wide object detected!!\n\r");

                    if (object1.midpoint) { //object 1 has already been found
                        object1.objectSize = rangeOfObject;
                        object1.midpoint = (startingDegree+endingDegree)/2;
                        object1.distanceToObject = averageOfThree; //only the distance to the last angle

                    }else { //object 1 has not been found
                        object2.objectSize = rangeOfObject;
                        object2.midpoint = (startingDegree+endingDegree)/2;
                        object2.distanceToObject = averageOfThree; //only the distance to the last angle
                    }
                }
            }

            //if (object1.objectSize > object2.objectSize) { //object 1 is larger
            //    turn_left(sensor_data, object2.midpoint-90);//turn to midpoint of object2
            //} else { //object2 is larger
            //    turn_left(sensor_data, object1.midpoint-90);//turn to midpoint of object1
            //}


        }
    }

    //PART3 DETECT THE SMALLEST WIDTH OBJECT


    //PART4 DRIVE TO THE SMALLEST WIDTH OBJECT (extra credit)


    oi_free(sensor_data); // do this once at end of main()

    return;
}


