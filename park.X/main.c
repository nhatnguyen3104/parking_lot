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


volatile uint16_t servo1_delay_ticks = 0; 
volatile uint8_t  servo2_delay_ticks = 0; 

uint8_t servo1_open = 0;
uint8_t servo2_open = 0;
uint8_t servo2_waiting = 0;   
int count =0;

void Interrupts_Init(void) {
    INTCONbits.GIE = 1;     
    INTCONbits.PEIE = 1;    
    PIE1bits.TMR1IE = 1;    
    PIR1bits.TMR1IF = 0;    
}

void Timer1_Init(void) {
    
    T1CONbits.TMR1ON = 0;   
    T1CONbits.TMR1CS = 0;   
    T1CONbits.T1CKPS = 0b11; 
    
    TMR1H = 0xE7;           
    TMR1L = 0x96;           
    
    T1CONbits.TMR1ON = 1;   
}

// --- HÀM MAIN CHÍNH ---
void main(void) {
    TRISD = 0x00;
    TRISB = 0xF0;
    TRISAbits.TRISA0 = 1;
    
    ESP32_UART_Init();
    Servo1_Init();
    Servo2_Init();
    RFID_Init();
    Sensor_Init();
    
    Timer1_Init();
    Interrupts_Init();
    
    Servo1_Close();
    Servo2_Close();

    while(1) {
        ESP32_UART_Update();
        
        
        if (servo1_delay_ticks == 0) {
            if (!ESP32_AllFull()) {
                uint8_t cardFound = RFID_CheckCard();
                
                if (cardFound && !servo1_open &&(count<4)) {
                    Servo1_Open();
                    servo1_open = 1;
                    servo1_delay_ticks = 100; // (100 ticks * 10ms = 1000ms)
                }

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
            if(!servo2_open) {
                Servo2_Open();
                servo2_open = 1;
            }
            servo2_waiting = 0;
            servo2_delay_ticks = 0; 
        } 
        else {
            if(servo2_open && !servo2_waiting) {
                servo2_waiting = 1;
                servo2_delay_ticks = 200;
            }
        }
        
        if(servo2_waiting && servo2_delay_ticks == 0) {
            Servo2_Close();
            servo2_open = 0;
            servo2_waiting = 0;
            count--;
        }

    }
}

void __interrupt() ISR(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;    
        TMR1H = 0xE7;           
        TMR1L = 0x96;
        
   
        

        if (servo1_delay_ticks > 0) {
            servo1_delay_ticks--;
        }
        

        if (servo2_delay_ticks > 0) {
            servo2_delay_ticks--;
        }
    }
    
   
}