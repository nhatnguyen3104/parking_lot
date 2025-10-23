/* 
 * File:   servo_close.h
 * Author: Admin
 *
 * Created on October 9, 2025, 8:55 PM
 */

#ifndef SERVO_CLOSE_H
#define	SERVO_CLOSE_H

#include <xc.h>
#include <stdint.h>

// Function prototypes
void Servo_Init(void);
void Servo_SetAngle(unsigned int angle);
void Servo_Open(void);
void Servo_Close(void);

#endif

#endif	/* SERVO_CLOSE_H */

