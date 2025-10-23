#ifndef SENSOR_H
#define SENSOR_H

#include <xc.h>
#include "config.h"


// Function prototypes
void Sensor_Init(void);
uint8_t Sensor_ReadObstacle(void);  // ??c c?m bi?n RB0
uint8_t Sensor_ReadIR(void);        // ??c c?m bi?n RA0

#endif