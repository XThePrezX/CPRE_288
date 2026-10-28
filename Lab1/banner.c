/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"


int main (void) {
    char str[] = "                    C";
    char display[21];
    int i = 0;
    int j;

    lcd_init();
    timer_init();

    while(1){
        int k = 0;

        for(j=i; j<i+20; j++){
            display[k] = str[j];
            k++;
        }


        lcd_puts(display);
        timer_waitMicros(300000);
        i++;
        lcd_init();
        if (i>sizeof(str)) {
            i=0;
        }
    }
    return 0;
}
