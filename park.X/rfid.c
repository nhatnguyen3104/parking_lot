#include "rfid.h"

#define _XTAL_FREQ 20000000

// Hàm ghi register
void RFID_Write(uint8_t addr, uint8_t data) {
    RFID_SDA = 0;
    
    // send address
    uint8_t address = (addr << 1) & 0x7E;
    for(uint8_t i = 0; i < 8; i++) {
        RFID_SCK = 0;
        RFID_MOSI = (address & 0x80) ? 1 : 0;
        address <<= 1;
        RFID_SCK = 1;
    }
    
    // send data
    for(uint8_t i = 0; i < 8; i++) {
        RFID_SCK = 0;
        RFID_MOSI = (data & 0x80) ? 1 : 0;
        data <<= 1;
        RFID_SCK = 1;
    }
    
    RFID_SDA = 1;
}

//  register
uint8_t RFID_Read(uint8_t addr) {
    uint8_t data = 0;
    RFID_SDA = 0;
    
    // send address
    uint8_t address = ((addr << 1) & 0x7E) | 0x80;
    for(uint8_t i = 0; i < 8; i++) {
        RFID_SCK = 0;
        RFID_MOSI = (address & 0x80) ? 1 : 0;
        address <<= 1;
        RFID_SCK = 1;
    }
    
    // ??c data
    for(uint8_t i = 0; i < 8; i++) {
        RFID_SCK = 0;
        RFID_SCK = 1;
        data <<= 1;
        if(RFID_MISO) data |= 1;
    }
    
    RFID_SDA = 1;
    return data;
}

// Kh?i t?o RFID
void RFID_Init(void) {
    // Setup wire
    RFID_SDA_DIR = 0;
    RFID_SCK_DIR = 0;
    RFID_MOSI_DIR = 0;
    RFID_MISO_DIR = 1;
    RFID_RST_DIR = 0;
    
    // Reset RFID
    RFID_RST = 0;
    __delay_ms(10);
    RFID_RST = 1;
    __delay_ms(10);
    
    // Soft reset
    RFID_Write(CommandReg, PCD_RESETPHASE);
    __delay_ms(10);
    
    // Reset wire
    while(RFID_Read(CommandReg) & (1 << 4)) {
        __delay_ms(1);
    }
    
    //configuration
    RFID_Write(TModeReg, 0x8D);
    RFID_Write(TPrescalerReg, 0x3E);
    RFID_Write(TReloadRegL, 30);
    RFID_Write(TReloadRegH, 0);
    
    RFID_Write(TxAutoReg, 0x40);
    RFID_Write(ModeReg, 0x3D);
    
    // B?t antenna
    RFID_Write(TxControlReg, RFID_Read(TxControlReg) | 0x03);
    
    __delay_ms(10);
}

// Ki?m tra th?
uint8_t RFID_CheckCard(void) {
    uint8_t buffer[2];
    uint16_t backBits;
    
    RFID_Write(BitFramingReg, 0x07);
    
    buffer[0] = PICC_CMD_REQA;
    uint8_t status = RFID_ToCard(PCD_TRANSCEIVE, buffer, 1, buffer, &backBits);
    
    return (status == 0 && backBits == 0x10) ? 1 : 0;
}

// Hàm giao ti?p v?i th?
uint8_t RFID_ToCard(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen) {
    uint8_t status = 1;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;
    
    if(command == PCD_TRANSCEIVE) {
        irqEn = 0x77;
        waitIRq = 0x30;
    }
    
    RFID_Write(ComIEnReg, irqEn | 0x80);
    RFID_Write(ComIrqReg, 0x7F);
    RFID_Write(FIFOLevelReg, 0x80);
    
    RFID_Write(CommandReg, PCD_IDLE);
    
    // Ghi data vào FIFO
    for(i = 0; i < sendLen; i++) {
        RFID_Write(FIFODataReg, sendData[i]);
    }
    
    // Th?c thi command
    RFID_Write(CommandReg, command);
    
    if(command == PCD_TRANSCEIVE) {
        RFID_Write(BitFramingReg, RFID_Read(BitFramingReg) | 0x80);
    }
    
    // Ch? hoàn t?t
    i = 1000;
    while(i > 0) {
        n = RFID_Read(ComIrqReg);
        if(n & waitIRq) break;
        if(n & 0x01) break;
        i--;
        __delay_ms(1);
    }
    
    RFID_Write(BitFramingReg, RFID_Read(BitFramingReg) & ~0x80);
    
    if(i > 0) {
        if(!(RFID_Read(ErrorReg) & 0x1B)) {
            status = 0;
            
            if(command == PCD_TRANSCEIVE) {
                n = RFID_Read(FIFOLevelReg);
                lastBits = RFID_Read(ControlReg) & 0x07;
                
                if(lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }
                
                if(n > 16) n = 16;
                for(i = 0; i < n; i++) {
                    backData[i] = RFID_Read(FIFODataReg);
                }
            }
        }
    }
    
    return status;
}