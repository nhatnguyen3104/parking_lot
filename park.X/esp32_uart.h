#ifndef ESP32_UART_H
#define ESP32_UART_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 20000000

void ESP32_UART_Init(void);
void ESP32_UART_Update(void);
uint8_t ESP32_AllFull(void);

#endif
