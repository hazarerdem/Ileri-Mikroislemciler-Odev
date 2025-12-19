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
#include "driverlib/adc.h"

int saniye=0,dakika=0,saat=0;

int saatonlar,saatbirler,dakikaonlar,dakikabirler,saniyeonlar,saniyebirler;

int temp,sicak;//tempbirler,temponlar,tempyuzler;


uint32_t gelenveri[4];
uint32_t ortdeger;

void Timerakesmesi();
void Timervekesmeayar();
void TimerYaz();
void ADCveKesmeAyar();
void ADCKesme();
void printTemp(int sicak);

int main(void) {

    Lcd_init();
    Timervekesmeayar();
    ADCveKesmeAyar();

    Lcd_Goto(1,0);
    Lcd_Puts("Dijital Saat");

    TimerEnable(TIMER0_BASE, TIMER_A);
    ADCProcessorTrigger(ADC0_BASE, 1);

    while (1) {

        TimerYaz();
        printTemp(sicak);

    }
}

void timerakesmesi(void){

    TimerIntClear(TIMER0_BASE, TIMER_A);

    saniye++;

    if(saniye==60)
    {
        saniye=0;
        dakika++;

        if(dakika==60)
        {
            dakika=0;
            saat++;

            if(saat==24)
            {
                saat=0;

            }
        }

    }

}

void Timervekesmeayar(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1); // TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1);

    IntMasterEnable(); //global
    IntEnable(INT_TIMER0A); //global

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerakesmesi);

}

void TimerYaz(){
    saatonlar=saat/10+48;
    saatbirler=saat%10+48;
    dakikaonlar=dakika/10+48;
    dakikabirler=dakika%10+48;
    saniyeonlar=saniye/10+48;
    saniyebirler=saniye%10+48;

    Lcd_Goto(2,0);
    Lcd_Putch(saatonlar);
    Lcd_Goto(2,1);
    Lcd_Putch(saatbirler);
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

void ADCveKesmeAyar(void){



    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    IntEnable(INT_ADC0SS0); //2global

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    //tek çevrim modu olarak seq=1 ayarladýk. þimdi seq=1 in steplerini ayarlamaya baþla
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    //Step konfigurasyon burada

    ADCIntEnable(ADC0_BASE, 1); //2 lokal
    ADCIntRegister(ADC0_BASE, 1, ADCKesme); //2 lokal


    ADCSequenceEnable(ADC0_BASE, 1); //adc çalýþ demedik, sadece adc artýk kullanýlabilir.

     //adc yi çevrime baþlat
}

void ADCKesme(){
    ADCSequenceDataGet(ADC0_BASE, 1, gelenveri);

    ortdeger = (gelenveri[0]+gelenveri[1]+gelenveri[2]+gelenveri[3])/4;


    temp=(ortdeger*200)/4095;
    sicak = temp-100;

    ADCIntClear(ADC0_BASE, 1);

    ADCProcessorTrigger(ADC0_BASE, 1); //adc yi tekrar çevrime baþlat.

}

void printTemp(int sicak)
{
    int negatif = 0;
    if (sicak < 0)
    {
        negatif = 1;
        sicak = -sicak;
    }
    int yuzler = sicak / 100;
    int onlar  = (sicak / 10) % 10;
    int birler = sicak % 10;
    Lcd_Goto(2, 10);
    if (negatif)
        Lcd_Putch('-');
    else
        Lcd_Putch(' ');
    if (sicak >= 100)
    {
        Lcd_Putch(yuzler + 48);
        Lcd_Putch(onlar + 48);
        Lcd_Putch(birler + 48);
    }
    else if (sicak >= 10)
    {
        Lcd_Putch(' ');
        Lcd_Putch(onlar + 48);
        Lcd_Putch(birler + 48);
    }
    else
    {
        Lcd_Putch(' ');
        Lcd_Putch(' ');
        Lcd_Putch(birler + 48);
    }
    Lcd_Putch('C');
}
