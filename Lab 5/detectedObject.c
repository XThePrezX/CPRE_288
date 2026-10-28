#include "custom.h"
#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include <stdio.h>
#include <string.h>

struct detectedObject {
    int objectSize;
    int midpoint;
    float distanceToObject;
};
