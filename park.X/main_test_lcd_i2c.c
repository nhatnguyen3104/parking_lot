#include <xc.h>
#include "lcd_i2c.h"

// C?u hình PIC16F877A
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main(void) {
    TRISD = 0x00;
    
    LCD_I2C_Init();
    
    int counter1 = 0, counter2 = 0, counter3 = 0, counter4 = 0;
    
    while(1) {
        // Hi?n th? tiêu ??
        LCD_I2C_Display4Sections("TEMP", "HUMI", "PRESS", "LIGHT");
        __delay_ms(2000);
        
        LCD_I2C_Clear();
        
        // Hi?n th? s? ??m
        for(int i = 0; i < 5; i++) {
            LCD_I2C_DisplayCounter4Sections(counter1++, counter2++, counter3++, counter4++);
            __delay_ms(1000);
        }
        
        LCD_I2C_Clear();
        
        // Hi?n th? giá tr? ??n gi?n
        LCD_I2C_SetCursor(0, 0);
        LCD_I2C_Print("T:25C");
        LCD_I2C_SetCursor(0, 8);
        LCD_I2C_Print("H:60%");
        LCD_I2C_SetCursor(1, 0);
        LCD_I2C_Print("P:1013");
        LCD_I2C_SetCursor(1, 8);
        LCD_I2C_Print("L:500");
        
        __delay_ms(3000);
        LCD_I2C_Clear();
    }
}