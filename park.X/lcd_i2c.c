#include "lcd_i2c.h"

// Hàm private
static void I2C_Init(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Write(unsigned char data);
static void LCD_Write4Bits(unsigned char data);
static void LCD_WriteCmd(unsigned char cmd);
static void LCD_WriteData(unsigned char data);

// Hàm I2C bit-banging
static void I2C_Init(void) {
    TRISD4 = 1;  // SDA (RC6) as input
    TRISD6 = 1;  // SCL (RC7) as input
    RD4 = 1;     // SDA high
    RD6 = 1;     // SCL high
}

static void I2C_Delay(void) {
    __delay_us(2);  // Gi?m delay ?? t?i ?u
}

static void I2C_Start(void) {
    TRISD4 = 0;  // SDA as output
    RD4 = 1;
    RD6 = 1;
    I2C_Delay();
    RD4 = 0;
    I2C_Delay();
    RD6 = 0;
    I2C_Delay();
}

static void I2C_Stop(void) {
    TRISD4 = 0;
    RD4 = 0;
    RD6 = 1;
    I2C_Delay();
    RD4 = 1;
    I2C_Delay();
    TRISD4 = 1;
}

static void I2C_WriteBit(unsigned char bit_val) {  // S?a: dùng unsigned char thay vì __bit
    TRISD4 = 0;
    RD4 = bit_val;
    I2C_Delay();
    RD6 = 1;
    I2C_Delay();
    RD6 = 0;
    I2C_Delay();
}

static void I2C_Write(unsigned char data) {
    unsigned char i;
    
    // Write 8 bits
    for(i = 0; i < 8; i++) {
        I2C_WriteBit((data & 0x80) ? 1 : 0);  // S?a: dùng toán t? 3 ngôi
        data <<= 1;
    }
    
    // Read ACK bit (??n gi?n hóa)
    TRISD4 = 1;
    I2C_Delay();
    RD6 = 1;
    I2C_Delay();
    RD6 = 0;
    I2C_Delay();
}

// Ghi 4 bit d? li?u ??n LCD (t?i ?u hóa)
static void LCD_Write4Bits(unsigned char data) {
    data |= LCD_BL;
    I2C_Start();
    I2C_Write(LCD_I2C_ADDR << 1);
    I2C_Write(data | LCD_EN);
    I2C_Write(data & ~LCD_EN);
    I2C_Stop();
}

// Ghi l?nh ??n LCD (t?i ?u hóa)
static void LCD_WriteCmd(unsigned char cmd) {
    LCD_Write4Bits(cmd & 0xF0);
    LCD_Write4Bits((cmd << 4) & 0xF0);
}

// Ghi d? li?u ??n LCD (t?i ?u hóa)
static void LCD_WriteData(unsigned char data) {
    LCD_Write4Bits((data & 0xF0) | LCD_RS);
    LCD_Write4Bits(((data << 4) & 0xF0) | LCD_RS);
}

// ========== PUBLIC FUNCTIONS ==========

// Kh?i t?o LCD (t?i ?u hóa)
void LCD_I2C_Init(void) {
    __delay_ms(50);
    I2C_Init();
    
    // Kh?i t?o 4-bit mode (rút g?n)
    LCD_Write4Bits(0x30);
    __delay_ms(5);
    LCD_Write4Bits(0x30);
    __delay_us(100);
    LCD_Write4Bits(0x20);
    __delay_us(100);
    
    // C?u hình LCD
    LCD_WriteCmd(0x28); // 4-bit, 2 dòng
    LCD_WriteCmd(0x0C); // Display on, cursor off
    LCD_WriteCmd(0x06); // Entry mode
    LCD_WriteCmd(0x01); // Clear
    __delay_ms(2);
}

// Xóa màn hình
void LCD_I2C_Clear(void) {
    LCD_WriteCmd(0x01);
    __delay_ms(2);
}

// ??t v? trí con tr? (t?i ?u hóa)
void LCD_I2C_SetCursor(unsigned char row, unsigned char col) {
    LCD_WriteCmd(0x80 + (row ? 0x40 : 0x00) + col);
}

// In chu?i (t?i ?u hóa)
void LCD_I2C_Print(const char *str) {
    while(*str) LCD_WriteData(*str++);
}

// In s? nguyên (t?i ?u hóa - s?a warning)
void LCD_I2C_PrintNum(int num) {
    char buffer[6];
    char *ptr = buffer + 5;
    *ptr = '\0';
    
    if (num == 0) {
        LCD_WriteData('0');
        return;
    }
    
    if (num < 0) {
        LCD_WriteData('-');
        num = -num;
    }
    
    // Chuy?n s? thành chu?i
    while(num > 0) {
        *--ptr = '0' + (unsigned char)(num % 10);  // S?a: ép ki?u ?? tránh warning
        num /= 10;
    }
    
    LCD_I2C_Print(ptr);
}

// ========== FUNCTIONS FOR 4 SECTIONS ==========

// Hi?n th? n?i dung ? 4 ph?n (t?i ?u hóa)
void LCD_I2C_Display4Sections(const char *part1, const char *part2, const char *part3, const char *part4) {
    LCD_I2C_SetCursor(0, 0);
    LCD_I2C_Print(part1);
    LCD_I2C_SetCursor(0, 8);
    LCD_I2C_Print(part2);
    LCD_I2C_SetCursor(1, 0);
    LCD_I2C_Print(part3);
    LCD_I2C_SetCursor(1, 8);
    LCD_I2C_Print(part4);
}

// Hi?n th? s? ??m ? 4 ph?n (t?i ?u hóa)
void LCD_I2C_DisplayCounter4Sections(int count1, int count2, int count3, int count4) {
    LCD_I2C_SetCursor(0, 0);
    LCD_I2C_Print("C1:");
    LCD_I2C_PrintNum(count1);
    
    LCD_I2C_SetCursor(0, 8);
    LCD_I2C_Print("C2:");
    LCD_I2C_PrintNum(count2);
    
    LCD_I2C_SetCursor(1, 0);
    LCD_I2C_Print("C3:");
    LCD_I2C_PrintNum(count3);
    
    LCD_I2C_SetCursor(1, 8);
    LCD_I2C_Print("C4:");
    LCD_I2C_PrintNum(count4);
}