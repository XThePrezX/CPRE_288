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


int main (void) {


    cyBOT_Scan_t scanner;
//    calloc(1, sizeof(cyBOT_Scan_t));

    cyBot_uart_init();

    lcd_init();

    cyBOT_init_Scan(0b0111);

    while (1) {
        int i;
        for (i=0; i<181; i+=10) {
            cyBOT_Scan(i, &scanner);
            char str[20] = {0};
            float h = scanner.sound_dist;
            sprintf(str, "%f", h);
            lcd_printf(str);
        }
    }

//    free(scanner);
    return 0;
}


