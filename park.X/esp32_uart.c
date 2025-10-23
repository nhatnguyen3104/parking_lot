#include "esp32_uart.h"
#include <stdio.h>
#include <string.h>

static uint8_t all_full = 0;
static char buffer[20];
static uint8_t idx = 0;

void ESP32_UART_Init(void) {
    TRISC6 = 0; // TX
    TRISC7 = 1; // RX
    SPBRG = 129; // Baudrate 9600 @20MHz
    BRGH = 1;
    SYNC = 0;
    SPEN = 1;
    TXEN = 1;
    CREN = 1;
    
    //LED
    TRISBbits.TRISB4 = 0; // LED FULL
    TRISBbits.TRISB5 = 0; // LED EMPTY
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
}

static uint8_t UART_DataReady(void) {
    return RCIF;
}

static uint8_t UART_Read(void) {
    while (!RCIF);
    return RCREG;
}

void ESP32_UART_Update(void) {
    if (UART_DataReady()) {
        char c = UART_Read();

        if (c == '\n') {
            buffer[idx] = '\0';
            idx = 0;

            uint8_t s1, s2, s3, s4;
            if (sscanf(buffer, "%hhu,%hhu,%hhu,%hhu", &s1, &s2, &s3, &s4) == 4) {
                if (s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1){
                    all_full = 1;
                    PORTBbits.RB4 = 1;
                    PORTBbits.RB5 = 0;
                }
                    
                else{
                    all_full = 0;
                    PORTBbits.RB4 = 0;
                    PORTBbits.RB5 = 1;
                }
                                 
            }
        } 
        else if (idx < sizeof(buffer) - 1) {
            buffer[idx++] = c;
        }
    }
}

uint8_t ESP32_AllFull(void) {
    return all_full;
}
