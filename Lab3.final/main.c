#include <string.h>
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include "custom.h"
#include "cyBot_Scan.h"
#include <detectedObject.h>


int floatcomp(const void* elem1, const void* elem2) //comparator for sorting array of floats; used to find median of scanner data
{
    if(*(const float*)elem1 < *(const float*)elem2)
        return -1;
    return *(const float*)elem1 > *(const float*)elem2;
}


void main (void) {

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    cyBot_uart_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    cyBOT_Scan_t scanner;
    calloc(1, sizeof(cyBOT_Scan_t));
    cyBOT_init_Scan(0b0111);

    float threeFloats[3];

    while (1) {
        int i;
        bool detectingObject = false;
        int startingDegree=0, endingDegree=0;
        int smallestObjMidpoint = 90;
        int smallestObjSize = 1000; //want the first scanned object to be the smallest (it will be less than 180 degrees)
        int currentObjMidpoint = 0;
        double currentObjSize = 0;
        for (i = 0; i<181; i+=1) { //scans every 1 degree from 0 to 180 degrees (inclusive)
            cyBOT_Scan(i, &scanner);

            int avgIndex = i%3;
            threeFloats[avgIndex] = scanner.sound_dist;

            if (avgIndex == 2) { //find the average of the three floats stored in threeFloats

                qsort(threeFloats, 3, sizeof(float), floatcomp); //sorts the array of floats using the above float comparator

                float medianOfThree = threeFloats[1]; //medianOfThree is now effective sensor distance & i-1 is degree of sensor

                //Prints sensor values to Putty
                char strDistance[20] = {0};
                float distance = medianOfThree;
                sprintf(strDistance, "%f", distance);
                char strDegree[20] = {0};
                int u = i-1;
                sprintf(strDegree, "%d", u);
                PuttyString(strDistance);
                PuttyString("cm @ ");
                PuttyString(strDegree);
                PuttyString(" degrees\n\r");

                if (medianOfThree < 100) {
                    if ( detectingObject == false) {//object has started being detected
                        detectingObject = true;
                        startingDegree = i-1;
                    }
                } else {
                    if (detectingObject == true){
                        detectingObject = false;
                        endingDegree = i-1;

                        char strRangeOfObject[20] = {0}; //print detected object range to putty
                        int rangeOfObject = endingDegree-startingDegree;
                        sprintf(strRangeOfObject, "%d", rangeOfObject);
                        PuttyString(strRangeOfObject);
                        PuttyString(" degree wide object detected!!\n\r");
                        currentObjMidpoint = endingDegree - rangeOfObject/2;
                        currentObjSize = distance*sin(((double)rangeOfObject*3.14)/180);
                        PuttyString("It is ");
                        char str5[20] = {0};
                        sprintf(str5, "%lf", currentObjSize);
                        PuttyString(str5);
                        PuttyString("cm wide!!\r\n");
                        if ((currentObjSize < smallestObjSize) && (currentObjSize > 10)){
                            smallestObjSize = currentObjSize;
                            smallestObjMidpoint = currentObjMidpoint;
                        }

                    }
                }
            }
        }

        char str1[20] = {0}; //print detected object range to putty
        sprintf(str1, "%d", smallestObjSize);
        PuttyString("---------- Smallest object size is ");
        PuttyString(str1);
        PuttyString(" cm wide.  It's midpoint it at ");
        char str2[20] = {0}; //print detected object range to putty
        sprintf(str2, "%d", smallestObjMidpoint);
        PuttyString(str2);
        PuttyString(" degrees! ----------\n\r");

        int amountToTurn = (smallestObjMidpoint-90)/2;
        if (amountToTurn < 0) {
            PuttyString("---------- Turning right ");
            char str3[20] = {0}; //print detected object range to putty
            sprintf(str3, "%d", amountToTurn*(-1));
            PuttyString(str3);
            PuttyString(" degrees!! ----------\n\r");
            turn_right(sensor_data, (-1)*amountToTurn);
        }
        if (amountToTurn > 0) {
            PuttyString("---------- Turning left ");
            char str4[20] = {0}; //print detected object range to putty
            sprintf(str4, "%d", amountToTurn);
            PuttyString(str4);
            PuttyString(" degrees!! ----------\n\r");
            turn_left(sensor_data, amountToTurn);
        }



    }

    oi_free(sensor_data); // do this once at end of main()

    return;

}


