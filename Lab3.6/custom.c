#include "custom.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include <stdio.h>
#include <string.h>


void SendCharToCybot() {
    char h[20];
    int x = cyBot_getByte();
    sprintf(h, "%c", x);
    //char j[] = "Got an ";
    //char* o[] = strcat(j, h);

    lcd_puts(h);
    cyBot_sendByte(h[0]);
}


void PuttyString(char h[]) {
    int i;
    for (i=0; i<strlen(h); i++) {
        cyBot_sendByte(h[i]);
    }


}
