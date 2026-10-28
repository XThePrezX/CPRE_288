/*
 * movement.h
 *
 *  Created on: Feb 4, 2024
 *      Author: pgask26
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"


//Function headers and macro definitions
double move_forward (oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor, double degrees);
double turn_left(oi_t *sensor, double degrees);
double move_backwards (oi_t *sensor_data, double distance_mm);


#endif /* MOVEMENT_H_ */
