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

    Lcd_Goto(1,0);              //lcd nin 1.satýr 0. sütununa gidip
    Lcd_Puts("Dijital Saat");   //dijital saat yaz

    TimerEnable(TIMER0_BASE, TIMER_A); //Timer0_A çevrime baþlat
    ADCProcessorTrigger(ADC0_BASE, 1); //adc yi çevrime baþlat

    while (1) {

        TimerYaz();
        printTemp(sicak);

    }
}

void timerakesmesi(void){

    TimerIntClear(TIMER0_BASE, TIMER_A);

    saniye++;               //her timer kesmesi geldiðinde yani 1 sn dolduðunda saniye birer artsýn
                            //saniye 60 olduðu anda saniye sýfýrlansýn dakika bir artsýn
    if(saniye==60)          //dakika 60 olduðu anda dakika sýfýrlansýn saat bir artsýn
    {                       //saat 24 olduðu anda sýfýrlansýn
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // timer hayat verdik

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // 40 000 000 dan 0 doðru insin
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1); // TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1);

    IntMasterEnable(); //global
    IntEnable(INT_TIMER0A); //global

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  //timer_a timeout olduðunda kesme olsun
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerakesmesi); // timer kesmesi geldiðinde "timerakesmesi" fonksiyonuna git

}

void TimerYaz(){
    saatonlar=saat/10+48;           //saat dakika ve saniyeyi 10 a böldüðümüzde onlar basamaðýný elder ederiz
    saatbirler=saat%10+48;          //mod 10 aldýðýmýzda 10 bölümünden kalaný verir ,birler basamaðýný elde ederiz
    dakikaonlar=dakika/10+48;       //bu elde ettiklerimizi 30H(48D) ile toplayarak ascii formatta lcd ye yazdýrýrýz
    dakikabirler=dakika%10+48;
    saniyeonlar=saniye/10+48;
    saniyebirler=saniye%10+48;

    Lcd_Goto(2,0);                  // saat dakika saniyenin, onlar ve birler basamaðýný ayrý ayrý yazdýracaðýz
    Lcd_Putch(saatonlar);           // 2.satýr 0. sütundan baþlayarak teker teker yaz
    Lcd_Goto(2,1);                  // belirtilen yerlere saonlar sabirler:dkonlar dkbirler:snonlar snbirler
    Lcd_Putch(saatbirler);          // þeklinde yaz lcd ye yaz
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // analog giriþi pe3 e baðlayacaðýmýz için portE ye hayat verdik
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);  // adc0 a hayat verdik
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // portE nin pin 3 ü adc nin analog giriþi olarak seçtik

    IntEnable(INT_ADC0SS0); //2 global ayardan biri, diðeri IntMasterEnable(); onu zaten timer için önceden açmýþtýk.

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    //tek çevrim modu olarak seq=1 ayarladýk. þimdi seq=1 in steplerini ayarlamaya baþla
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0); // 1. sequencenin 0-1-2-3. stepleri
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0); // ch0 yani pe3 den veri alsýn
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    //Step konfigurasyon burada

    ADCIntEnable(ADC0_BASE, 1); //2 lokal ayar
    ADCIntRegister(ADC0_BASE, 1, ADCKesme); //2 lokal ayar . adc kesmesi geldiðinde "ADCKesme" fonksiyonuna git


    ADCSequenceEnable(ADC0_BASE, 1); //adc çalýþ demedik, sadece adc artýk kullanýlabilir.
                                     //çalýþmasý içi ADCProcessorTrigger(ADC0_BASE, 1); yapmalýyýz
                                     //baþta yaptýk
}

void ADCKesme(){
    ADCSequenceDataGet(ADC0_BASE, 1, gelenveri);    //adc0 ýn 1.seq inden gelen deðerleri "gelenveri" dizisine koy

    ortdeger = (gelenveri[0]+gelenveri[1]+gelenveri[2]+gelenveri[3])/4;
    //gelen 4 deðeri toplayýp 4 e bölerek ortalamasýný al

    temp=(ortdeger*200)/4095;//12 bit adc nin çözünürlüðü 4095,dijital veriyi anlamlý bir sýcaklýk verisine benzettim.
    sicak = temp-100;       //hesaplanan deðerden 100 çýkararak gerçek sýcaklýk bulunur (-100C ile +100C arasýnda)

    ADCIntClear(ADC0_BASE, 1); //kesmeyi temizleyelim ki iþlemci sürekli ayný kesmeye girip kitlenmesin

    ADCProcessorTrigger(ADC0_BASE, 1); //adc yi tekrar çevrime baþlat.

}

void printTemp(int sicak)
{
    int negatif = 0; // Sýcaklýðýn negatif olup olmadýðýný tutan bayrak (flag)

    // 1. Ýþaret Kontrolü: Eðer sayý negatifse pozitife çevirip iþaret bilgisini saklýyoruz
    if (sicak < 0)
    {
        negatif = 1;      // Negatif olduðunu iþaretle
        sicak = -sicak;   // Matematiksel iþlemler için sayýyý pozitife çevir
    }

    // 2. Basamak Ayýrma: Sayýyý yüzler, onlar ve birler basamaðýna parçalýyoruz
    int yuzler = sicak / 100;           // Yüzler basamaðýný bulur (Örn: 125 -> 1)
    int onlar  = (sicak / 10) % 10;     // Onlar basamaðýný bulur (Örn: 125 -> 2)
    int birler = sicak % 10;            // Birler basamaðýný bulur (Örn: 125 -> 5)

    // LCD üzerinde yazýnýn baþlayacaðý koordinatý ayarla (2. satýr, 10. sütun)
    Lcd_Goto(2, 10);

    // 3. Ýþaret Yazdýrma: Negatifse '-' iþareti, pozitifse boþluk koyar
    if (negatif)
        Lcd_Putch('-');
    else
        Lcd_Putch(' ');

    // 4. Dinamik Hizalama ve Karakter Dönüþümü (+48 ASCII'deki rakam karþýlýðýdýr)
    if (sicak >= 100)
    {
        // Sayý 3 basamaklý ise (Örn: -125 veya 125)
        Lcd_Putch(yuzler + 48); // ASCII tablosunda '0' karakteri 48'den baþlar
        Lcd_Putch(onlar + 48);
        Lcd_Putch(birler + 48);
    }
    else if (sicak >= 10)
    {
        // Sayý 2 basamaklý ise baþýna bir boþluk ekle (Örn: " 25")
        Lcd_Putch(' ');
        Lcd_Putch(onlar + 48);
        Lcd_Putch(birler + 48);
    }
    else
    {
        // Sayý tek basamaklý ise baþýna iki boþluk ekle (Örn: "  5")
        Lcd_Putch(' ');
        Lcd_Putch(' ');
        Lcd_Putch(birler + 48);
    }

    // Sonuna birim (Celsius) ekle
    Lcd_Putch('C');
}
