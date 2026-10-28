#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <adc.h>
#include <math.h>

int main(void) {
	timer_init();
	lcd_init();
	adc_init();

	while(1){
	    uint32_t rawvalue = adc_read();
	    float convertedvalue = pow(((float)rawvalue)/13489.0, -1.5);
	    printf("%f\n", convertedvalue);
	}

}
