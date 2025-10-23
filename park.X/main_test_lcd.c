#include <xc.h>
#include "config.h"
#include "lcd_sensor.h"
#include "servo.h"
#include "rfid.h"
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

// Bi?n cho servo
uint8_t servo1_open = 0;
uint8_t servo2_open = 0;
uint32_t last_object_time = 0;

void main(void) {
    // Kh?i t?o các module
    lcd_init();
    Servo1_Init();
    Servo2_Init();
    RFID_Init();
    Sensor_Init();
    
    // ?óng servo ban ??u
    Servo1_Close();
    Servo2_Close();
    
    unsigned int counter = 0;
    unsigned char last_s1 = 0, last_s2 = 0, last_s3 = 0, last_s4 = 0;
    
    // Hi?n th? tr?ng thái ban ??u c?a sensor
    last_s1 = IR_SENSOR_1;
    last_s2 = IR_SENSOR_2;
    last_s3 = IR_SENSOR_3;
    last_s4 = IR_SENSOR_4;
    
    lcd_update_sensor_status(1, last_s1);
    lcd_update_sensor_status(2, last_s2);
    lcd_update_sensor_status(3, last_s3);
    lcd_update_sensor_status(4, last_s4);

    while(1) {
        counter++;
        
        // === PH?N 1: C?P NH?T 4 SENSOR TRÊN RB4-7 ===
        unsigned char s1 = IR_SENSOR_1;
        unsigned char s2 = IR_SENSOR_2;
        unsigned char s3 = IR_SENSOR_3;
        unsigned char s4 = IR_SENSOR_4;
        
        // Ch? update khi có thay ??i
        if(s1 != last_s1) {
            lcd_update_sensor_status(1, s1);
            last_s1 = s1;
        }
        if(s2 != last_s2) {
            lcd_update_sensor_status(2, s2);
            last_s2 = s2;
        }
        if(s3 != last_s3) {
            lcd_update_sensor_status(3, s3);
            last_s3 = s3;
        }
        if(s4 != last_s4) {
            lcd_update_sensor_status(4, s4);
            last_s4 = s4;
        }
        
        // === PH?N 2: ?I?U KHI?N SERVO ===
        
        // Servo 1: RFID + Obstacle Sensor
        static uint16_t rfid_counter = 0;
        rfid_counter++;
        if(rfid_counter >= 20) { // Ki?m tra RFID m?i 200ms
            if(RFID_CheckCard() && !servo1_open) {
                Servo1_Open();
                servo1_open = 1;
            }
            rfid_counter = 0;
        }
        
        // Servo 1: ?óng b?ng obstacle sensor
        if(servo1_open && Sensor_ReadObstacle() == 1) {
            Servo1_Close();
            servo1_open = 0;
        }
        
        // Servo 2: IR Sensor + t? ??ng ?óng
        if(Sensor_ReadIR() == 1) {
            if(!servo2_open) {
                Servo2_Open();
                servo2_open = 1;
            }
            last_object_time = counter;
        }
        
        // Servo 2: T? ??ng ?óng sau 4 giây (400 * 10ms)
        if(servo2_open && (counter - last_object_time >= 400)) {
            Servo2_Close();
            servo2_open = 0;
        }
        
        // === PH?N 3: HI?N TH? HO?T ??NG ===
        if(counter % 100 == 0) {
            lcd_goto(0x4D);
            if(counter % 200 == 0) {
                lcd_puts("*");
            } else {
                lcd_puts(" ");
            }
        }
        
        // Reset counter n?u quá l?n
        if(counter > 60000) counter = 0;
        
        __delay_ms(10);
    }
}