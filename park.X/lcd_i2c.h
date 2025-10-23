#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <xc.h>

#define _XTAL_FREQ 20000000

// ??a ch? I2C c?a LCD
#define LCD_I2C_ADDR 0x27

// Các chân ?i?u khi?n
#define LCD_RS 0x01
#define LCD_EN 0x04
#define LCD_BL 0x08

// Ch? gi? l?i các hàm c?n thi?t
void LCD_I2C_Init(void);
void LCD_I2C_Clear(void);
void LCD_I2C_SetCursor(unsigned char row, unsigned char col);
void LCD_I2C_Print(const char *str);
void LCD_I2C_PrintNum(int num);
void LCD_I2C_Display4Sections(const char *part1, const char *part2, const char *part3, const char *part4);
void LCD_I2C_DisplayCounter4Sections(int count1, int count2, int count3, int count4);

#endif