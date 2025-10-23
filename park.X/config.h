#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#define _XTAL_FREQ 20000000


//LED
#define LED1_DIR TRISBbits.TRISB4
#define LED1 PORTBbits.RB4
#define LED2_DIR TRISBbits.TRISB5
#define LED2 PORTBbits.RB5
// LCD pins on PORTD
#define LCD_RS  PORTDbits.RD0
#define LCD_EN  PORTDbits.RD1

#define LCD_D4  PORTDbits.RD4
#define LCD_D5  PORTDbits.RD5  
#define LCD_D6  PORTDbits.RD6
#define LCD_D7  PORTDbits.RD7

#define LCD_DELAY_US __delay_us(100)
#define LCD_DELAY_MS __delay_ms(2)

// RFID RC522 Pin Definitions
#define RFID_SDA     PORTCbits.RC0
#define RFID_SCK     PORTCbits.RC3
#define RFID_MOSI    PORTCbits.RC5
#define RFID_MISO    PORTCbits.RC4
#define RFID_RST     PORTBbits.RB3

#define RFID_SDA_DIR TRISCbits.TRISC0
#define RFID_SCK_DIR TRISCbits.TRISC3
#define RFID_MOSI_DIR TRISCbits.TRISC5
#define RFID_MISO_DIR TRISCbits.TRISC4
#define RFID_RST_DIR TRISBbits.TRISB3


// Servo Definitions
#define SERVO1_PIN    PORTCbits.RC1
#define SERVO1_DIR    TRISCbits.TRISC1

#define SERVO2_PIN    PORTCbits.RC2
#define SERVO2_DIR    TRISCbits.TRISC2

// SENSOR trên PORTB
#define IR_SENSOR_1  PORTBbits.RB4  // sensor 1
#define IR_SENSOR_2  PORTBbits.RB5  // sensor 2  
#define IR_SENSOR_3  PORTBbits.RB6  // sensor 3
#define IR_SENSOR_4  PORTBbits.RB7  // sensor 4

#define IR_SENSOR_1_DIR TRISBbits.TRISB4
#define IR_SENSOR_2_DIR TRISBbits.TRISB5  
#define IR_SENSOR_3_DIR TRISBbits.TRISB6
#define IR_SENSOR_4_DIR TRISBbits.TRISB7

// Obstacle Sensor Definitions
#define OBSTACLE_SENSOR PORTBbits.RB0
#define OBSTACLE_DIR    TRISBbits.TRISB0

#define IR_SENSOR     PORTAbits.RA4
#define IR_SENSOR_DIR TRISAbits.TRISA4

#define ESP_TX  RC6  // ESP8266 TX ? PIC RX
#define ESP_RX  RC7  // ESP8266 RX ? PIC TX

// LCD positions for 4 quadrants
#define LCD_TOP_LEFT     0x80
#define LCD_TOP_RIGHT    0x88
#define LCD_BOTTOM_LEFT  0xC0
#define LCD_BOTTOM_RIGHT 0xC8

// RFID Commands
#define PCD_IDLE              0x00
#define PCD_AUTHENT           0x0E
#define PCD_RECEIVE           0x08
#define PCD_TRANSMIT          0x04
#define PCD_TRANSCEIVE        0x0C
#define PCD_RESETPHASE        0x0F
#define PCD_CALCCRC           0x03

// MFRC522 Registers
#define CommandReg            0x01
#define ComIEnReg             0x02
#define DivIEnReg             0x03
#define ComIrqReg             0x04
#define DivIrqReg             0x05
#define ErrorReg              0x06
#define Status1Reg            0x07
#define Status2Reg            0x08
#define FIFODataReg           0x09
#define FIFOLevelReg          0x0A
#define WaterLevelReg         0x0B
#define ControlReg            0x0C
#define BitFramingReg         0x0D
#define CollReg               0x0E
#define ModeReg               0x11
#define TxControlReg          0x14
#define TxAutoReg             0x15
#define TxSelReg              0x16
#define RxSelReg              0x17
#define RxThresholdReg        0x18
#define DemodReg              0x19
#define MfTxReg               0x1C
#define MfRxReg               0x1D
#define SerialSpeedReg        0x1F
#define CRCResultRegH         0x21
#define CRCResultRegL         0x22
#define ModWidthReg           0x24
#define RFCfgReg              0x26
#define GsNReg                0x27
#define CWGsCfgReg            0x28
#define ModGsCfgReg           0x29
#define TModeReg              0x2A
#define TPrescalerReg         0x2B
#define TReloadRegH           0x2C
#define TReloadRegL           0x2D
#define TCounterValueRegH     0x2E
#define TCounterValueRegL     0x2F

// MIFARE Commands
#define PICC_CMD_REQA         0x26
#define PICC_CMD_ANTICOLL     0x93
#define PICC_CMD_SEL          0x93
#define PICC_CMD_AUTH_KEYA    0x60
#define PICC_CMD_READ         0x30
#define PICC_CMD_HALT         0x50


#endif