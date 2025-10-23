#include <xc.h>
#include "config.h"
#include "rfid.h"
#include "servo.h"
#include "sensor.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000

uint8_t servo1_open = 0;
uint8_t servo2_open = 0;
uint16_t servo2_timer = 0;
uint8_t servo2_waiting = 0;
int count =0;


void main(void) {
    TRISD = 0x00;
    TRISB = 0xF0;
    TRISAbits.TRISA0 = 1;
    
    Servo1_Init();
    Servo2_Init();
    RFID_Init();
    Sensor_Init();
    
    LED1_DIR =0;
    LED2_DIR=0;
    LED1=0;
    LED2=0;
    
    Servo1_Close();
    Servo2_Close();
    
    
   

    while(1) {
        //LED
        if(count<4){
            LED1=1;
            LED2=0;
        }
        else{
            LED1=0;
            LED2=1;
        }
        // SERVO 1
        uint8_t cardFound = RFID_CheckCard();
        if(cardFound && !servo1_open &&(count<4)) {
            Servo1_Open();
            servo1_open = 1;
            __delay_ms(1000);
        }
        
        if(servo1_open && Sensor_ReadObstacle() == 1) {
            Servo1_Close();
            servo1_open = 0;
            count++;
            __delay_ms(1000);
        }
        
        // SERVO 2
        uint8_t object_detected = Sensor_ReadIR();
        
        if(object_detected == 1) {
            if(!servo2_open) {
                Servo2_Open();
                servo2_open = 1;
            }
            servo2_waiting = 0;
            servo2_timer = 0;
        } 
        else {
            if(servo2_open && !servo2_waiting) {
                servo2_waiting = 1;
                servo2_timer = 0;
            }
        }
        
        if(servo2_waiting) {
            servo2_timer++;
            if(servo2_timer >= 100) {
                Servo2_Close();
                count--;
                servo2_open = 0;
                servo2_waiting = 0;
                servo2_timer = 0;
            }
        }
        
       
        
        __delay_ms(10);
    }
}
