#include "lcd_sensor.h"

// Hàm delay ??n gi?n, không dùng hàm delay c?a compiler
void lcd_delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 1000; j++);
}

void lcd_delay_us(unsigned int us) {
    while(us--) {
        __delay_us(1);
    }
}

void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    
    // Ghi 4 bit cao
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    LCD_EN = 1;
    lcd_delay_us(10);
    LCD_EN = 0;
    lcd_delay_us(100);
    
    // Ghi 4 bit th?p  
    PORTD = (PORTD & 0x0F) | ((cmd << 4) & 0xF0);
    LCD_EN = 1;
    lcd_delay_us(10);
    LCD_EN = 0;
    lcd_delay_ms(2); // Delay dài h?n cho l?nh
}

void lcd_data(unsigned char data) {
    LCD_RS = 1;
    
    // Ghi 4 bit cao
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    LCD_EN = 1;
    lcd_delay_us(10);
    LCD_EN = 0;
    lcd_delay_us(100);
    
    // Ghi 4 bit th?p
    PORTD = (PORTD & 0x0F) | ((data << 4) & 0xF0);
    LCD_EN = 1;
    lcd_delay_us(10);
    LCD_EN = 0;
    lcd_delay_us(100);
}

void lcd_goto(unsigned char pos) {
    lcd_cmd(pos | 0x80);
}

void lcd_puts(const char *str) {
    unsigned char i = 0;
    while(str[i] != 0 && i < 16) { // Gi?i h?n 16 ký t?
        lcd_data(str[i]);
        i++;
    }
}

void lcd_clear(void) {
    lcd_cmd(0x01);
    lcd_delay_ms(2);
}

void lcd_init(void) {
    TRISD = 0x00; // PORTD output
    TRISB = 0xF0; // RB4-7 input
    
    lcd_delay_ms(50); // Ch? LCD ?n ??nh
    
    // Kh?i t?o LCD 4-bit mode
    LCD_RS = 0;
    LCD_EN = 0;
    
    // Sequence kh?i t?o chu?n
    PORTD = 0x30;
    LCD_EN = 1; lcd_delay_us(1); LCD_EN = 0;
    lcd_delay_ms(5);
    
    PORTD = 0x30;
    LCD_EN = 1; lcd_delay_us(1); LCD_EN = 0;
    lcd_delay_us(100);
    
    PORTD = 0x30;
    LCD_EN = 1; lcd_delay_us(1); LCD_EN = 0;
    lcd_delay_us(100);
    
    PORTD = 0x20; // Chuy?n sang 4-bit mode
    LCD_EN = 1; lcd_delay_us(1); LCD_EN = 0;
    lcd_delay_us(100);
    
    // C?u hình LCD
    lcd_cmd(0x28); // 4-bit, 2 dòng, 5x8 font
    lcd_cmd(0x0C); // Display on, cursor off
    lcd_cmd(0x06); // Entry mode increment
    lcd_clear();   // Clear display
    
    // Hi?n th? layout c? ??nh
    lcd_goto(LCD_TOP_LEFT);
    lcd_puts("1:----");
    lcd_goto(LCD_TOP_RIGHT);
    lcd_puts("2:----");
    lcd_goto(LCD_BOTTOM_LEFT);
    lcd_puts("3:----");
    lcd_goto(LCD_BOTTOM_RIGHT);
    lcd_puts("4:----");
}

void lcd_update_sensor_status(unsigned char sensor_num, unsigned char status) {
    unsigned char position;
    const char *text;
    
    // Xác ??nh v? trí và text
    switch(sensor_num) {
        case 1: 
            position = LCD_TOP_LEFT + 2;
            text = (status == 1) ? "FILL " : "EMPTY";
            break;
        case 2:
            position = LCD_TOP_RIGHT + 2;
            text = (status == 1) ? "FILL " : "EMPTY";
            break;
        case 3:
            position = LCD_BOTTOM_LEFT + 2;
            text = (status == 1) ? "FILL " : "EMPTY";
            break;
        case 4:
            position = LCD_BOTTOM_RIGHT + 2;
            text = (status == 1) ? "FILL " : "EMPTY";
            break;
        default:
            return;
    }
    
    // C?p nh?t LCD
    lcd_goto(position);
    lcd_puts(text);
}