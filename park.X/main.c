#include <xc.h>
#include "config.h"
#include "rfid.h"
#include "servo.h"
#include "sensor.h"
#include "esp32_uart.h"

// --- C?u h�nh ---
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

// --- Bi?n to�n c?c cho b? ??m th?i gian (thay th? cho delay) ---
// volatile: B�o cho tr�nh bi�n d?ch bi?t bi?n n�y c� th? thay ??i trong ng?t
// 1 tick = 10ms (???c ??nh ngh?a trong Timer1_Init)
volatile uint16_t servo1_delay_ticks = 0; // B? ??m cho Servo 1 (100 ticks = 1000ms)
volatile uint8_t  servo2_delay_ticks = 0; // B? ??m cho Servo 2 (5 ticks = 50ms)

// --- Bi?n tr?ng th�i ---
uint8_t servo1_open = 0;
uint8_t servo2_open = 0;
// uint16_t servo2_timer = 0;     // KH�NG C?N N?A
uint8_t servo2_waiting = 0;     // V?n d�ng ?? l�m c? tr?ng th�i
int count =0;

// --- C?u h�nh ng?t ---
void Interrupts_Init(void) {
    INTCONbits.GIE = 1;     // Cho ph�p ng?t to�n c?c
    INTCONbits.PEIE = 1;    // Cho ph�p ng?t ngo?i vi
    PIE1bits.TMR1IE = 1;    // Cho ph�p ng?t Timer1
    PIR1bits.TMR1IF = 0;    // X�a c? ng?t Timer1
}

// --- C?u h�nh Timer1 (T?o nh?p 10ms) ---
void Timer1_Init(void) {
    // Fosc = 20MHz -> Fosc/4 = 5MHz (Chu k? 0.2us)
    // Prescaler 1:8 -> Timer clock = 5MHz / 8 = 625,000 Hz (Chu k? 1.6us)
    // C?n 10ms (10,000us) -> S? tick c?n = 10,000us / 1.6us = 6250 ticks
    // Timer1 l� 16-bit, ??m L�N. C?n n?p gi� tr? 65536 - 6250 = 59286
    // 59286 = 0xE796
    
    T1CONbits.TMR1ON = 0;   // T?t Timer1
    T1CONbits.TMR1CS = 0;   // D�ng clock n?i Fosc/4
    T1CONbits.T1CKPS = 0b11; // Prescaler 1:8
    
    TMR1H = 0xE7;           // N?p gi� tr? cao
    TMR1L = 0x96;           // N?p gi� tr? th?p
    
    T1CONbits.TMR1ON = 1;   // B?t Timer1
}

// --- H�M MAIN CH�NH ---
void main(void) {
    TRISD = 0x00;
    TRISB = 0xF0;
    TRISAbits.TRISA0 = 1;
    
    // Kh?i t?o c�c module
    ESP32_UART_Init();
    Servo1_Init();
    Servo2_Init();
    RFID_Init();
    Sensor_Init();
    
    // Kh?i t?o Timer v� Ng?t
    Timer1_Init();
    Interrupts_Init();
    
    // ??t tr?ng th�i ban ??u
    Servo1_Close();
    Servo2_Close();

    while(1) {
        //Update data from ESP32(IR1,2,3,4)
        ESP32_UART_Update();
        
        // --- LOGIC SERVO 1 (NON-BLOCKING) ---
        
        // Ch? x? l� logic Servo 1 n?u n� kh�ng trong th?i gian "ch?" (cooldown)
        if (servo1_delay_ticks == 0) {
            if (!ESP32_AllFull()) {
                uint8_t cardFound = RFID_CheckCard();
                
                // M? servo
                if (cardFound && !servo1_open &&(count<4)) {
                    Servo1_Open();
                    servo1_open = 1;
                    // THAY V� delay 1s -> B?t ??u ??m 1s
                    servo1_delay_ticks = 100; // (100 ticks * 10ms = 1000ms)
                }

                // ?�ng servo
                if (servo1_open && Sensor_ReadObstacle() == 1) {
                    Servo1_Close();
                    servo1_open = 0;
                    count++;
                    servo1_delay_ticks = 100; // (100 ticks * 10ms = 1000ms)
                }
            } else {
                if (servo1_open) {
                    Servo1_Close();
                    servo1_open = 0;
                    servo1_delay_ticks = 100;
                }
            }
        }
        
        
        // --- LOGIC SERVO 2 (NON-BLOCKING) ---
        uint8_t object_detected = Sensor_ReadIR();
        
        if(object_detected == 1) {
            // C� v?t c?n
            if(!servo2_open) {
                Servo2_Open();
                servo2_open = 1;
            }
            // Reset b? ??m (n?u ?ang ??m) v� v?t v?n c�n ?�
            servo2_waiting = 0;
            servo2_delay_ticks = 0; 
        } 
        else {
            // Kh�ng c� v?t c?n
            if(servo2_open && !servo2_waiting) {
                servo2_waiting = 1;
                servo2_delay_ticks = 200;
            }
        }
        
        // Ki?m tra n?u ?ang trong tr?ng th�i ch? V� b? ??m ?� v? 0
        if(servo2_waiting && servo2_delay_ticks == 0) {
            // ?� h?t 50ms, ?�ng servo
            Servo2_Close();
            servo2_open = 0;
            servo2_waiting = 0;
            count--;
        }

    }
}

void __interrupt() ISR(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;    // X�a c? ng?t ?? s?n s�ng cho l?n sau
        TMR1H = 0xE7;           // N?p l?i gi� tr? cho timer
        TMR1L = 0x96;
        
        // --- X? l� c�c b? ??m ph?n m?m ---
        
        // ??m ng??c cho Servo 1
        if (servo1_delay_ticks > 0) {
            servo1_delay_ticks--;
        }
        
        // ??m ng??c cho Servo 2
        if (servo2_delay_ticks > 0) {
            servo2_delay_ticks--;
        }
    }
    
   
}