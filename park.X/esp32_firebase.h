#ifndef ESP32_FIREBASE_H
#define ESP32_FIREBASE_H

#include "config.h"

// Khai báo hàm cho ESP32
void ESP32_Init(void);
void ESP32_SendCommand(const char *cmd);
void ESP32_SendDataToFirebase(int sensor1, int sensor2, int sensor3, int sensor4);
void ESP32_UpdateFirebase(int s1, int s2, int s3, int s4);
unsigned char ESP32_WaitResponse(const char *expected, unsigned int timeout);

#endif