#include "servo.h"

// Servo 1 (Gate) - RC1
void Servo1_Init(void) {
    SERVO1_DIR = 0;  // Output
    SERVO1_PIN = 0;  // Ban ??u ? m?c th?p
}

void Servo1_Open(void) {
    // Góc 90 ?? - M? c?a
    SERVO1_PIN = 1;
    __delay_us(1500); // 1.5ms pulse
    SERVO1_PIN = 0;
    __delay_us(18500); // T?ng 20ms period
}

void Servo1_Close(void) {
    // Góc 0 ?? - ?óng c?a  
    SERVO1_PIN = 1;
    __delay_us(500);  // 0.5ms pulse
    SERVO1_PIN = 0;
    __delay_us(19500); // T?ng 20ms period
}

// Servo 2 (Barrier) - RC2
void Servo2_Init(void) {
    SERVO2_DIR = 0;  // Output
    SERVO2_PIN = 0;  // Ban ??u ? m?c th?p
}

void Servo2_Open(void) {
    // Góc 90 ?? - M? barrier
    SERVO2_PIN = 1;
    __delay_us(1500); // 1.5ms pulse
    SERVO2_PIN = 0;
    __delay_us(18500); // T?ng 20ms period
}

void Servo2_Close(void) {
    // Góc 0 ?? - ?óng barrier
    SERVO2_PIN = 1;
    __delay_us(500);  // 0.5ms pulse
    SERVO2_PIN = 0;
    __delay_us(19500); // T?ng 20ms period
}