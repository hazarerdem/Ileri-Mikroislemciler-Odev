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
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );   //lcd nin datasheetinden baþlangýç ayarlarý
    GPIOPinWrite(LCDPORT, E, E);                        //30 hex gönderip aç-kapa yapmalýyýz
    SysCtlDelay(1000);                                  //bu iþlemi 3 kere tekrarlamalýyýz
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
    Lcd_Komut(0x28);   // 4bit, 2 line, 5x10 pixel
    Lcd_Komut(0x0C);   // display ON, cursor off, blink off
    Lcd_Komut(0x18);   // cursor kaydýr
    Lcd_Komut(0x06);   // her yazma iþleminden sonra cursor pozisyonunu arttýr
    Lcd_Temizle();
}


void Lcd_Komut(unsigned char c) {

    // ==== Üst nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xF0) >> 2 ); // D4-D5-D6-D7 yi PortB nin 2-3-4-5. pinlerine atadaðýmýz için
    GPIOPinWrite(LCDPORT, RS, 0x00); // C nin high kýsmýný alýp 2 bit saða ötelediðimizde xx10 11xx gibi olur
    GPIOPinWrite(LCDPORT, E, E);     // istediðimiz gibi 2-3-4-5. pinlerine denk geliyor bilgiler
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);     // komutu yollamak için enable pinini aç-kapa yapýyoruz
    SysCtlDelay(1000);
    SysCtlDelay(5000);

    // ==== Alt nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0F) << 2 ); //ayný þekilde low kýsmý alýp 2 bit sola ötelediðimizde
    GPIOPinWrite(LCDPORT, RS, 0x00);                            // PORTB nin 2-3-4-5. pinlerine geliyor
    GPIOPinWrite(LCDPORT, E, E);                                // komutu yollamak için enable pinini aç-kapa yapýyoruz
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);
}


void Lcd_Putch(unsigned char d) {

    // ==== Üst nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xF0) >> 2 );// maskeleme ve öteleme mantýðý
    GPIOPinWrite(LCDPORT, RS, 0x01);                           // yukardakiyle ayný mantýkla
    GPIOPinWrite(LCDPORT, E, E);                               // komutu yollamak için enable pinini aç-kapa yapýyoruz
    SysCtlDelay(1000);
    GPIOPinWrite(LCDPORT, E, 0);
    SysCtlDelay(1000);
    SysCtlDelay(5000);

    // ==== Alt nibble ====
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0F) << 2 );// maskeleme ve öteleme mantýðý
    GPIOPinWrite(LCDPORT, RS, 0x01);                           // yukardakiyle ayný mantýkla
    GPIOPinWrite(LCDPORT, E, E);                               // komutu yollamak için enable pinini aç-kapa yapýyoruz
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
        address = 0x80 + y; //lcd 1. satýr 80 hexden baþlar bu yüzden 0x80 + y
    else
        address = 0xC0 + y ;//lcd 2. satýr C0 hexden baþlar

    Lcd_Komut(address);
}


void Lcd_Temizle(void) {
    Lcd_Komut(0x01);    //temizleme komutu lcd nin datasheetinde 0x01 yollanarak oluyor
    SysCtlDelay(20000);
}
