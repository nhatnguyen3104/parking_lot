#ifndef SERVO_H
#define SERVO_H

#include <xc.h>
#include <stdint.h>
#include "config.h"

// Function prototypes for Servo 1 (RFID Gate)
void Servo1_Init(void);
void Servo1_Open(void);
void Servo1_Close(void);

// Function prototypes for Servo 2 (IR Sensor)
void Servo2_Init(void);
void Servo2_Open(void);
void Servo2_Close(void);

#endif