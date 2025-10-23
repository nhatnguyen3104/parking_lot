#include "lcd_4quadrant.h"
#include "config.h"

// Hàm n?i b? - không khai báo trong .h
static void lcd_cmd(unsigned char cmd) {
    LCD_RS = 0;
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0; __delay_us(100);
    PORTD = (PORTD & 0x0F) | ((cmd & 0x0F) << 4);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0; __delay_us(100);
}

static void lcd_data(unsigned char data) {
    LCD_RS = 1;
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0; __delay_us(100);
    PORTD = (PORTD & 0x0F) | ((data & 0x0F) << 4);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0; __delay_us(100);
}

static void lcd_goto(unsigned char pos) {
    lcd_cmd(pos);
}

static void lcd_puts(const char *str) {
    while(*str) {
        lcd_data(*str++);
    }
}

static void lcd_putint(int num) {
    if (num == 0) {
        lcd_data('0');
        return;
    }
    
    if (num < 0) {
        lcd_data('-');
        num = -num;
    }
    
    // Hi?n th? s?
    int divisor = 10000;
    int started = 0;
    
    while (divisor > 0) {
        int digit = num / divisor;
        if (digit > 0 || started || divisor == 1) {
            lcd_data('0' + digit);
            started = 1;
        }
        num %= divisor;
        divisor /= 10;
    }
}

// Hàm chính - ch? có 3 hàm này ???c public
void lcd_init(void) {
    TRISD = 0x00;
    __delay_ms(20);
    
    PORTD = 0x20;
    LCD_EN = 1; __delay_us(10); LCD_EN = 0;
    __delay_ms(5);
    
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    __delay_ms(2);
}

void lcd_update_custom(unsigned char position, const char *text) {
    lcd_goto(position);
    lcd_puts(text);
}

void lcd_update_custom_int(unsigned char position, int value) {
    lcd_goto(position);
    lcd_putint(value);
}
void lcd_clear_area(unsigned char position, unsigned char length) {
    lcd_goto(position);
    for(unsigned char i = 0; i < length; i++) {
        lcd_data(' ');
    }
}
