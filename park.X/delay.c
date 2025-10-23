#include "delay.h"

void delay_ms(unsigned int ms) {
    for(unsigned int i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}

void delay_us(unsigned int us) {
    for(unsigned int i = 0; i < us; i++) {
        __delay_us(1);
    }
}