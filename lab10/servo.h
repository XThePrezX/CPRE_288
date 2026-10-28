/*
 * servo.h
 *
 *  Created on: Apr 12, 2024
 *      Author: pgask26
 */

#ifndef SERVO_H_
#define SERVO_H_
#include "stdint.h"


//Initalize the servo
void servo_init(void);

//Move the servo
void servo_move(uint16_t degrees);


#endif /* SERVO_H_ */
