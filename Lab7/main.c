#include <string.h>
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "uart.h"
#include "custom.h"
#include "cyBot_Scan.h"


int floatcomp(const void* elem1, const void* elem2) //comparator for sorting array of floats; used to find median of scanner data
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}

void main (void) {

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    cyBOT_Scan_t scanner;
    calloc(1, sizeof(cyBOT_Scan_t));
    cyBOT_init_Scan(0b0111);

    while (1) {

        int i;
        bool detectingObject = false;

        int startingDegree = 0;
        float startingDistance = 0.0;

        int endingDegree = 0;

        int currentObjMidpoint = 0;
        double currentObjSize = 0.0;

        double smallestObjSize = 100000.0; //want the first scanned object to be the smallest (it will be less than 180 degrees)*/
        int smallestObjMidpoint = 0;

        for (i = 0; i<181; i+=2) { //scans every 1 degree from 0 to 180 degrees (inclusive)
            cyBOT_Scan(i, &scanner);

            if (i == 0) {
                unsigned long w;
                for (w = 2000; w < 1000000; w++) {
                    // Do nothing, just consume CPU cycles
                }
            }

            //Set scanner distance and IR value
            float distance = scanner.sound_dist;
            int irDistence = scanner.IR_raw_val;

            //End on object and starting on object precaution (check this sometime)
            if (detectingObject == true && i < 7) {
                detectingObject = false;
            }

            //Prints sensor values to Putty
            char strDistance[20] = {0};
            sprintf(strDistance, "%f", distance);

            char strDegree[20] = {0};
            int deg = i;
            sprintf(strDegree, "%d", deg);

            PuttyString(strDistance);
            PuttyString("cm @ ");
            PuttyString(strDegree);
            PuttyString(" degrees\n\r");

            char stringIRDistence[20] = {0};
            sprintf(stringIRDistence, "%d", irDistence);
            PuttyString(stringIRDistence);
            PuttyString(" IR Distance\n\r\n\r");


            if (irDistence > 850 && i < 180) { //Greater than 850 when detecting an object

                if (detectingObject == false) {//object has started being detected

                    detectingObject = true;
                    startingDegree = i;
                    startingDistance = distance;
                }
            } else if (irDistence <= 850 || i >= 180) { //Less than 850 when not detecting an object

                if (detectingObject == true){

                    detectingObject = false;
                    endingDegree = i;

                    //print detected object range to putty
                    char strRangeOfObject[20] = {0};
                    int rangeOfObject = endingDegree - startingDegree;
                    sprintf(strRangeOfObject, "%d", rangeOfObject);
                    PuttyString(strRangeOfObject);
                    PuttyString(" degree wide object detected!!\n\r");

                    currentObjMidpoint = endingDegree - (rangeOfObject/2); //Possible Truncation
                    currentObjSize = sqrt(pow((double)startingDistance, 2.0) + pow((double)distance, 2.0) - 2 * (double)startingDistance * (double)distance * cos((double)rangeOfObject));

                    PuttyString("It is ");
                    char str5[20] = {0};
                    sprintf(str5, "%f", currentObjSize);
                    PuttyString(str5);
                    PuttyString("cm wide!!\r\n\r\n");

                    if (currentObjSize < smallestObjSize){
                        smallestObjSize = currentObjSize;
                        smallestObjMidpoint = currentObjMidpoint;
                    }
                }
            }
        }

        if (smallestObjSize != 100000.0) { //Scanning and finding nothing
            char str1[20] = {0}; //print detected object range to putty
            sprintf(str1, "%lf", smallestObjSize);
            PuttyString("---------- Smallest object size is ");
            PuttyString(str1);
            PuttyString(" cm wide.  It's midpoint it at ");
            char str2[20] = {0}; //print detected object range to putty
            sprintf(str2, "%d", smallestObjMidpoint);
            PuttyString(str2);
            PuttyString(" degrees! ----------\n\r");

            int amountToTurn = (smallestObjMidpoint);

            if (amountToTurn > 0) { //Cybot Left
                PuttyString("---------- Turning left ");
                char str4[20] = {0}; //print detected object range to putty
                sprintf(str4, "%d", amountToTurn);
                PuttyString(str4);
                PuttyString(" degrees!! ----------\n\r");
                turn_left(sensor_data, amountToTurn);
                //move_forward(sensor_data, smallestObjDistance*3.0);
            }

        }

    }

    //Bump
    oi_free(sensor_data); // do this once at end of main()

    return;

}


