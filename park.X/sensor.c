#include <xc.h>
#include "sensor.h"

#define _XTAL_FREQ 20000000

void Sensor_Init(void) {
    OBSTACLE_DIR = 1;  // Input
    IR_SENSOR_DIR = 1; // Input

}

uint8_t Sensor_ReadObstacle(void) {
    
    return (OBSTACLE_SENSOR == 0) ? 1 : 0;
}

uint8_t Sensor_ReadIR(void) {
    
    return (IR_SENSOR == 0) ? 1 : 0;
}