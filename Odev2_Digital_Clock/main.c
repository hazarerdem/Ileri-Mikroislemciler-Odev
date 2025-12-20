#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

int saniye;
int dakika;
int saat;

int saatonlar,saatbirler,dakikaonlar,dakikabirler,saniyeonlar,saniyebirler;

void Timerakesmesi();

void Timervekesmeayarý();



int main(void) {
    Lcd_init();
    Lcd_Goto(1,0);
    Lcd_Puts("Dijital Saat");

    saat=0;
    dakika=0;
    saniye=0;

    Timervekesmeayarý();


    while (1) {

        saatonlar=saat/10+48;       //saat dakika ve saniyeyi 10 a böldüðümüzde onlar basamaðýný elder ederiz
        saatbirler=saat%10+48;      //mod 10 aldýðýmýzda 10 bölümünden kalaný verir ,birler basamaðýný elde ederiz
        dakikaonlar=dakika/10+48;   //bu elde ettiklerimizi 30H(48D) ile toplayarak ascii formatta lcd ye yazdýrýrýz
        dakikabirler=dakika%10+48;
        saniyeonlar=saniye/10+48;
        saniyebirler=saniye%10+48;

        Lcd_Goto(2,0);
        Lcd_Putch(saatonlar);       // saat dakika saniyenin, onlar ve birler basamaðýný ayrý ayrý yazdýracaðýz
        Lcd_Goto(2,1);              // 2.satýr 0. sütundan baþlayarak ilgili yerlere teker teker yaz
        Lcd_Putch(saatbirler);      // araya : koyarak yaz
        Lcd_Goto(2,2);
        Lcd_Puts(":");
        Lcd_Goto(2,3);
        Lcd_Putch(dakikaonlar);
        Lcd_Goto(2,4);
        Lcd_Putch(dakikabirler);
        Lcd_Goto(2,5);
        Lcd_Puts(":");
        Lcd_Goto(2,6);
        Lcd_Putch(saniyeonlar);
        Lcd_Goto(2,7);
        Lcd_Putch(saniyebirler);

    }
}

void timerakesmesi(void){

    TimerIntClear(TIMER0_BASE, TIMER_A);

    saniye++;

    if(saniye==60)
    {
        saniye=0;       //her 1 timer kesmesi olduðunda
        dakika++;       //saniyeyi bir arttýr ,saniye 60 olduðunda saniyeyi sýfýrla ve dakikayý bir arttýr

        if(dakika==60)
        {
            dakika=0;
            saat++;     // dakika 60 olduðunda dakikayý sýfýrla ve saati bir arttýr

            if(saat==24)
            {
                saat=0; // saat 24 olduðunda saati sýfýrla

            }
        }

    }

}

void Timervekesmeayarý(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//timera hayat ver

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //aþþaðý sayýcý 40 000 000 dan aþþaðý doðru sayacak yani
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1); // TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1);

    IntMasterEnable(); //global
    IntEnable(INT_TIMER0A); //global

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //kesme timeout olduðunda olsun
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerakesmesi); //kesme geldiðnide "timerakesmesi" adlý fonksiyona gitsin
    TimerEnable(TIMER0_BASE, TIMER_A);//timer baþla
}
