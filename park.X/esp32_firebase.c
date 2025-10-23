#include "esp32_firebase.h"
#include <string.h>
#include <stdio.h>

// Kh?i t?o UART cho ESP32
void ESP32_Init(void) {
    TRISC6 = 1;  // RX input (nh?n t? ESP32)
    TRISC7 = 0;  // TX output (g?i ??n ESP32)
    
    // C?u hình UART 115200 baud (ESP32 th??ng dùng 115200)
    SPBRG = 32;   // For 20MHz, 115200 baud
    TXSTAbits.SYNC = 0;    // Asynchronous mode
    TXSTAbits.BRGH = 1;    // High baud rate
    RCSTAbits.SPEN = 1;    // Serial port enabled
    RCSTAbits.CREN = 1;    // Continuous reception
    TXSTAbits.TXEN = 1;    // Transmission enabled
}

// G?i chu?i l?nh ??n ESP32
void ESP32_SendCommand(const char *cmd) {
    while(*cmd) {
        while(!TXSTAbits.TRMT);  // Ch? buffer tr?ng
        TXREG = *cmd++;
    }
}

// G?i d? li?u sensor lên Firebase qua ESP32
void ESP32_SendDataToFirebase(int sensor1, int sensor2, int sensor3, int sensor4) {
    char buffer[200];
    
    // ??nh d?ng d? li?u JSON cho ESP32
    sprintf(buffer, "FIREBASE:%d,%d,%d,%d\r\n", 
            sensor1, sensor2, sensor3, sensor4);
    
    ESP32_SendCommand(buffer);
}

// Hàm update ??n gi?n
void ESP32_UpdateFirebase(int s1, int s2, int s3, int s4) {
    ESP32_SendDataToFirebase(s1, s2, s3, s4);
}

// Ch? ph?n h?i t? ESP32
unsigned char ESP32_WaitResponse(const char *expected, unsigned int timeout) {
    unsigned int time = 0;
    char response[100] = "";
    unsigned char index = 0;
    
    while(time < timeout) {
        if(PIR1bits.RCIF) {  // Có data nh?n ???c
            response[index++] = RCREG;
            response[index] = '\0';
            
            if(strstr(response, expected) != NULL) {
                return 1;  // Tìm th?y expected response
            }
            
            if(index >= 99) index = 0;
        }
        __delay_ms(1);
        time++;
    }
    return 0;
}