#ifndef LCD_SENSOR_H
#define LCD_SENSOR_H

#include "config.h"

void lcd_init(void);
void lcd_update_sensor_status(unsigned char sensor_num, unsigned char status);
void lcd_update_all_sensors(void);
unsigned char read_sensor(unsigned char sensor_num);
unsigned char read_sensor_direct(unsigned char sensor_num); 

void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_goto(unsigned char pos);
void lcd_puts(const char *str);
#endif