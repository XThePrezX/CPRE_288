/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "driverlib/interrupt.h"

// Uncomment or add any include directives that are needed

int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    ping_init();

    // YOUR CODE HERE
    int numOverflows = 0;

    while(1)
    {

        // YOUR CODE HERE
        char s[50];
        float distance = ping_getDistance();
        float time = distance / 1715;
        int cc = time / 0.000000625;

        if (distance == 0) {
            numOverflows++;
        }

        sprintf(s, "DIST = %lf\nPULSE = %lf\nCYCLES = %d\nOVERFLOWS = %d", distance, time, cc, numOverflows);
        lcd_printf(s);
        timer_waitMillis(100);


    }

}
