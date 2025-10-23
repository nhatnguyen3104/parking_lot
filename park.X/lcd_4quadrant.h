#ifndef LCD_4QUADRANT_H
#define LCD_4QUADRANT_H

#include <xc.h>
#include "config.h"

#define _XTAL_FREQ 20000000


// Ch? khai báo các hàm chính
void lcd_init(void);
void lcd_update_custom(unsigned char position, const char *text);
void lcd_update_custom_int(unsigned char position, int value);
void lcd_clear_area(unsigned char position, unsigned char length);

#endif