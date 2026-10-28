/*
 * movement.c
 *
 *  Created on: Feb 4, 2024
 *      Author: pgask26
 */

#include "movement.h"
#include "open_interface.h"

//i_setWheels() to move (-500 to 500)

//Takes distance in MM
double move_forward (oi_t *sensor_data, double distance_mm) {

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(500,500); //move forward at full speed
    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        printf("%lf \n", sum);

        if (sensor_data->bumpLeft) {
            move_backwards(sensor_data, 150);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 90);
            sum -= 250;
            oi_setWheels(500,500);
        }
        if (sensor_data->bumpRight) {
            move_backwards(sensor_data, 150);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 90);
            sum -= 250;
            oi_setWheels(500,500);
        }
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double move_backwards (oi_t *sensor_data, double distance_mm) {

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100,-100); //move forward at full speed
    while (sum > (-1)*distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        printf("%lf \n", sum);
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double turn_right(oi_t *sensor, double degrees) {

    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100,100); //move forward at full speed
    while (sum > (-1)*degrees) {
        oi_update(sensor);
        sum += sensor -> angle; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}

double turn_left(oi_t *sensor, double degrees) {
    // the following code could be put in function move_forward()
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(100,-100); //move forward at full speed
    while (sum < degrees) {
        oi_update(sensor);
        sum += sensor -> angle; // use -> notation since pointer
    }
    oi_setWheels(0,0); //stop
    return sum;
}
