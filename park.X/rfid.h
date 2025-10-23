#ifndef RFID_H
#define RFID_H

#include <xc.h>
#include <stdint.h>
#include "config.h"

// Function prototypes
void RFID_Init(void);
uint8_t RFID_CheckCard(void);
uint8_t RFID_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen);

#endif