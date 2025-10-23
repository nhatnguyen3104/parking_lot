#include <xc.h>
#include "sensor.h"

#define _XTAL_FREQ 20000000

void Sensor_Init(void) {
    OBSTACLE_DIR = 1;  // Input
    IR_SENSOR_DIR = 1; // Input

}

uint8_t Sensor_ReadObstacle(void) {
    // Tr? v? 1 n?u có v?t c?n, 0 n?u không có
    // Gi? s? c?m bi?n xu?t LOW khi có v?t
    return (OBSTACLE_SENSOR == 0) ? 1 : 0;
}

uint8_t Sensor_ReadIR(void) {
    // Tr? v? 1 n?u có v?t, 0 n?u không có
    // Gi? s? c?m bi?n xu?t HIGH khi có v?t
    return (IR_SENSOR == 0) ? 1 : 0;
}