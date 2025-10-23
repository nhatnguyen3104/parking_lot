#ifndef DELAY_H
#define DELAY_H

#include <xc.h>

#define _XTAL_FREQ 20000000

void delay_ms(unsigned int ms);
void delay_us(unsigned int us);

#endif