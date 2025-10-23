#include <xc.h>
#include "config.h"
#include "rfid.h"
#include "servo.h"
#include "sensor.h"
#include "esp32_uart.h"

// --- C?u hình ---
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

// --- Bi?n toàn c?c cho b? ??m th?i gian (thay th? cho delay) ---
// volatile: Báo cho trình biên d?ch bi?t bi?n này có th? thay ??i trong ng?t
// 1 tick = 10ms (???c ??nh ngh?a trong Timer1_Init)
volatile uint16_t servo1_delay_ticks = 0; // B? ??m cho Servo 1 (100 ticks = 1000ms)
volatile uint8_t  servo2_delay_ticks = 0; // B? ??m cho Servo 2 (5 ticks = 50ms)

// --- Bi?n tr?ng thái ---
uint8_t servo1_open = 0;
uint8_t servo2_open = 0;
// uint16_t servo2_timer = 0;     // KHÔNG C?N N?A
uint8_t servo2_waiting = 0;     // V?n dùng ?? làm c? tr?ng thái
int count =0;

// --- C?u hình ng?t ---
void Interrupts_Init(void) {
    INTCONbits.GIE = 1;     // Cho phép ng?t toàn c?c
    INTCONbits.PEIE = 1;    // Cho phép ng?t ngo?i vi
    PIE1bits.TMR1IE = 1;    // Cho phép ng?t Timer1
    PIR1bits.TMR1IF = 0;    // Xóa c? ng?t Timer1
}

// --- C?u hình Timer1 (T?o nh?p 10ms) ---
void Timer1_Init(void) {
    // Fosc = 20MHz -> Fosc/4 = 5MHz (Chu k? 0.2us)
    // Prescaler 1:8 -> Timer clock = 5MHz / 8 = 625,000 Hz (Chu k? 1.6us)
    // C?n 10ms (10,000us) -> S? tick c?n = 10,000us / 1.6us = 6250 ticks
    // Timer1 là 16-bit, ??m LÊN. C?n n?p giá tr? 65536 - 6250 = 59286
    // 59286 = 0xE796
    
    T1CONbits.TMR1ON = 0;   // T?t Timer1
    T1CONbits.TMR1CS = 0;   // Dùng clock n?i Fosc/4
    T1CONbits.T1CKPS = 0b11; // Prescaler 1:8
    
    TMR1H = 0xE7;           // N?p giá tr? cao
    TMR1L = 0x96;           // N?p giá tr? th?p
    
    T1CONbits.TMR1ON = 1;   // B?t Timer1
}

// --- HÀM MAIN CHÍNH ---
void main(void) {
    TRISD = 0x00;
    TRISB = 0xF0;
    TRISAbits.TRISA0 = 1;
    
    // Kh?i t?o các module
    ESP32_UART_Init();
    Servo1_Init();
    Servo2_Init();
    RFID_Init();
    Sensor_Init();
    
    // Kh?i t?o Timer và Ng?t
    Timer1_Init();
    Interrupts_Init();
    
    // ??t tr?ng thái ban ??u
    Servo1_Close();
    Servo2_Close();

    while(1) {
        //Update data from ESP32(IR1,2,3,4)
        ESP32_UART_Update();
        
        // --- LOGIC SERVO 1 (NON-BLOCKING) ---
        
        // Ch? x? lý logic Servo 1 n?u nó không trong th?i gian "ch?" (cooldown)
        if (servo1_delay_ticks == 0) {
            if (!ESP32_AllFull()) {
                uint8_t cardFound = RFID_CheckCard();
                
                // M? servo
                if (cardFound && !servo1_open &&(count<4)) {
                    Servo1_Open();
                    servo1_open = 1;
                    // THAY VÌ delay 1s -> B?t ??u ??m 1s
                    servo1_delay_ticks = 100; // (100 ticks * 10ms = 1000ms)
                }

                // ?óng servo
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
            // Có v?t c?n
            if(!servo2_open) {
                Servo2_Open();
                servo2_open = 1;
            }
            // Reset b? ??m (n?u ?ang ??m) vì v?t v?n còn ?ó
            servo2_waiting = 0;
            servo2_delay_ticks = 0; 
        } 
        else {
            // Không có v?t c?n
            if(servo2_open && !servo2_waiting) {
                servo2_waiting = 1;
                servo2_delay_ticks = 200;
            }
        }
        
        // Ki?m tra n?u ?ang trong tr?ng thái ch? VÀ b? ??m ?ã v? 0
        if(servo2_waiting && servo2_delay_ticks == 0) {
            // ?ã h?t 50ms, ?óng servo
            Servo2_Close();
            servo2_open = 0;
            servo2_waiting = 0;
            count--;
        }

    }
}

void __interrupt() ISR(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;    // Xóa c? ng?t ?? s?n sàng cho l?n sau
        TMR1H = 0xE7;           // N?p l?i giá tr? cho timer
        TMR1L = 0x96;
        
        // --- X? lý các b? ??m ph?n m?m ---
        
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