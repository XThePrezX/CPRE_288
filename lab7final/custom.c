#include "custom.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

void SendCharToCybot() {
    char h[20];
    int x = uart_receive();
    sprintf(h, "%c", x);
    //char j[] = "Got an ";
    //char* o[] = strcat(j, h);

    lcd_puts(h);
    uart_sendChar(h[0]);
}


void PuttyString(char h[]) {
    int i;
    for (i=0; i<strlen(h); i++) {
        uart_sendChar(h[i]);
    }


}
