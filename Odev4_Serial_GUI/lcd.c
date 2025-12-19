#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"

void Lcd_init(void) {
    SysCtlPeripheralEnable(LCDPORTENABLE);
    GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

    SysCtlDelay(50000);

    GPIOPinWrite(LCDPORT, RS,  0x00 );

    // ---- INIT SEQUENCE (8bit den 4 bit moda) ----
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(50000);

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(50000);

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(50000);

    // ---- NORMAL LCD INIT ----
    Lcd_Komut(0x28);   // 4bit, 2 line
    Lcd_Komut(0x0C);   // display ON
    Lcd_Komut(0x1C);
    Lcd_Komut(0x06);   // entry mode
    Lcd_Temizle();
    Lcd_Temizle();
    Lcd_Temizle();
}


void Lcd_Komut(unsigned char c) {

    // ==== Üst nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0) >> 2 );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);

    // ==== Alt nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0F) << 2 );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);
}


void Lcd_Putch(unsigned char d) {

    // ==== Üst nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xF0) >> 2 );
    GPIOPinWrite(LCDPORT, RS, 0x01);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);

    // ==== Alt nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0F) << 2 );
    GPIOPinWrite(LCDPORT, RS, 0x01);
    GPIOPinWrite(LCDPORT, E, E);
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);
}


void Lcd_Puts(char *s){
    while(*s)
        Lcd_Putch(*s++);
}


void Lcd_Goto(char x, char y) {
    unsigned char address;
    if(x == 1)
        address = 0x80 + y;
    else
        address = 0xC0 + y ;

    Lcd_Komut(address);
}


void Lcd_Temizle(void) {
    Lcd_Komut(0x01);
    SysCtlDelay(20000);
}
