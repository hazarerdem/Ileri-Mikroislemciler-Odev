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
#include "driverlib/uart.h"
#include "inc/hw_uart.h"
#include "driverlib/pin_map.h"

int saniye=0,dakika=0,saat=0;
int saatonlar,saatbirler,dakikaonlar,dakikabirler,saniyeonlar,saniyebirler;
int temp,sicak;
int a=0;
int b=0;
int i;

uint32_t gelenveri[4];
uint32_t ortdeger;
uint32_t uartveri[8];

void timerakesmesi();
void Timervekesmeayar();
void TimerYaz();
void ADCveKesmeAyar();
void ADCKesme();
void printTemp(int sicak);
void uartayar();
void butonayar();
void butonkesme();

    int main(void) {

    Lcd_init();
    Timervekesmeayar();
    ADCveKesmeAyar();
    uartayar();
    butonayar();

    Lcd_Goto(1,0);
    Lcd_Puts("Dijital Saat");

    TimerEnable(TIMER0_BASE, TIMER_A);// Timer0_A çevrime baþlat
    ADCProcessorTrigger(ADC0_BASE, 1);//adc yi çevrime baþlat

    while (1) {

        if(UARTCharsAvail(UART0_BASE)){

            // SAAT GÖNDERME ÝÞLEMÝ ÝÇÝN
                 b = UARTCharGet(UART0_BASE);
                 if (b == 0x23) { // ilk byte # gelirse
                     for ( i = 0; i < 8; ++i) {
                         uartveri[i] = UARTCharGet(UART0_BASE)-48;  //elde ettiðimiz veriyi 30H çýkartýrýz
                     }                                              //normal formatta uartveriye atarýz
                     saat = ((uartveri[0]*10)+uartveri[1]);
                     dakika = ((uartveri[3]*10)+uartveri[4]);       //uart verinin ilk biti saatin onlar basamaðý
                     saniye = ((uartveri[6]*10)+uartveri[7]);       //sonraki veri saatin birleri þeklinde ilerler...
                 }
            // HARF GÖNDERME ÝÞLEMÝ ÝÇÝN
                 else if (b == 0x2A) { // ilk byte * gelirse
                     GPIOPinWrite(GPIO_PORTF_BASE,6,2); //kýrmýzý ledi yak
                         for ( i = 0; i < 8; ++i) {
                             uartveri[i] = UARTCharGet(UART0_BASE); //sýrayla gelen verileri uartveri dizisine al
                             if (uartveri[i] == 0x2A){              //aldýðýn veri 0x2A (*) mý diye
                                 i=8;}                              //her alýþýnda kontrol et
                             else {                                 //eðer (*) sa aldýðýn deðer
                                 Lcd_Goto(1,13+i);                  //döngüden çýk 1.sütunda ilgili yere
                                 Lcd_Putch(uartveri[i]);}           //o veriyi yazmaya baþla
                         }
                }
             }

//        TimerYaz();
//        printTemp(sicak);

    }
}

void timerakesmesi(void){

    TimerIntClear(TIMER0_BASE, TIMER_A);
    saniye++;                  //saat dakika ve saniyeyi 10 a böldüðümüzde onlar basamaðýný elder ederiz
                               //mod 10 aldýðýmýzda 10 bölümünden kalaný verir ,birler basamaðýný elde ederiz
    if(saniye==60)             //bu elde ettiklerimizi 30H(48D) ile toplayarak ascii formatta lcd ye yazdýrýrýz
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
    TimerYaz();
    printTemp(sicak);
    UARTCharPut(UART0_BASE, 'T');           //burdan kontrol iþareti olarak T gönderiyoruz
    UARTCharPut(UART0_BASE, saatonlar);     //sharp develop programýnda T geldimi diye kontrol edicez
    UARTCharPut(UART0_BASE, saatbirler);    //T geldiyse textboxun içine yazdýrýcaz sharp developta
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, dakikaonlar);   //sonrasýnda saatin,dakikanýn ve saniyenin ilgili basamaklarýn
    UARTCharPut(UART0_BASE, dakikabirler);  //araya : koyarak sýrayla gönder
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, saniyeonlar);
    UARTCharPut(UART0_BASE, saniyebirler);
    UARTCharPut(UART0_BASE, '\n');         //sharp developta readline fonksiyonu
                                           //gönderilenleri okurken bu iþareti görünce bittiðini anlar
}

void Timervekesmeayar(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //timer0 hayat verdik

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);// aþþaðý doðru saysýn, 0 a doðru
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1); // TimerLoadSet(TIMER0_BASE, TIMER_A, 40000000-1);

    IntMasterEnable(); //global
    IntEnable(INT_TIMER0A); //global

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//timer timeout olduðunda kesme aktif olsun
    TimerIntRegister(TIMER0_BASE, TIMER_A, timerakesmesi);//kesme geldiðinde "timerakesmesi" fonksiyonuna gitsin

}

void TimerYaz(){
    saatonlar=saat/10+48;           //saat dakika ve saniyeyi 10 a böldüðümüzde onlar basamaðýný elder ederiz
    saatbirler=saat%10+48;          //mod 10 aldýðýmýzda 10 bölümünden kalaný verir ,birler basamaðýný elde ederiz
    dakikaonlar=dakika/10+48;       //bu elde ettiklerimizi 30H(48D) ile toplayarak ascii formatta lcd ye yazdýrýrýz
    dakikabirler=dakika%10+48;
    saniyeonlar=saniye/10+48;
    saniyebirler=saniye%10+48;

    Lcd_Goto(2,0);
    Lcd_Putch(saatonlar);           // saat dakika saniyenin, onlar ve birler basamaðýný ayrý ayrý yazdýracaðýz
    Lcd_Goto(2,1);                  // 2.satýr 0. sütundan baþlayarak ilgili yerlere teker teker yaz
    Lcd_Putch(saatbirler);          // araya : koyarak yaz
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

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);// analog giriþi pe3 e baðlayacaðýmýz için portE ye hayat verdik
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);// adc0 a hayat verdik
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);// portE nin pin 3 ü adc nin analog giriþi olarak seçtik

    IntEnable(INT_ADC0SS0);//2 global ayardan biri, diðeri IntMasterEnable(); onu zaten timer için önceden açmýþtýk.

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    //tek çevrim modu olarak seq=1 ayarladýk. þimdi seq=1 in steplerini ayarlamaya baþla
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);// 1. sequencenin 0-1-2-3. stepleri
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);// ch0 yani pe3 den veri alsýn
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    //Step konfigurasyon burada

    ADCIntEnable(ADC0_BASE, 1); //2 lokal ayar
    ADCIntRegister(ADC0_BASE, 1, ADCKesme); //2 lokal ayar. adc kesmesi geldiðinde "ADCKesme" fonksiyonuna git


    ADCSequenceEnable(ADC0_BASE, 1); //adc çalýþ demedik, sadece adc artýk kullanýlabilir.

}

void ADCKesme(){
    ADCSequenceDataGet(ADC0_BASE, 1, gelenveri);    //adc0 ýn sequence 1 inden gelen verileri "gelenveri" dizisine aktar

    ortdeger = (gelenveri[0]+gelenveri[1]+gelenveri[2]+gelenveri[3])/4; //gelenveri dizisindeki 4 deðeri toplayýp 4 e bölerek ortalamayý buluruz

    temp=(ortdeger*200)/4095;   // 12 bitlik adc nin çözünürlüðü 4095. dijital deðeri sýcaklýk deðerine benzetmeye çalýþtým
    sicak = temp-100;           // hesaplanan deðerden 100 çýkarýlarak gerçek sýcaklýk bulunur (-100C ile +100C arasýnda)

    ADCIntClear(ADC0_BASE, 1);  // kesmeyi temizliyoruz yoksa sürekli ayný kesmeye girer ve kilitlenir

    ADCProcessorTrigger(ADC0_BASE, 1); //adc yi tekrar çevrime baþlat.

}

void printTemp(int sicak)
{
    UARTCharPut(UART0_BASE, 'D');
    int negatif = 0;
    if (sicak < 0)                      //eðer sayý negatifse pozitife çevirip
    {                                   //matematiksel iþlemler için sayýyý saklýyoruz
        negatif = 1;
        sicak = -sicak;
    }
    int yuzler = sicak / 100;           // sayýyý yüzler onlar birler basamaðýna parçalýyoruz
    int onlar  = (sicak / 10) % 10;
    int birler = sicak % 10;
    Lcd_Goto(2, 10);                    // lcd nin belirtilen yerine git
    if (negatif)
    {
        Lcd_Putch('-');                 // negatifse - koy
        UARTCharPut(UART0_BASE, '-');
    }
    else                                // pozitifse boþluk koy
    {
        Lcd_Putch(' ');
        UARTCharPut(UART0_BASE, ' ');
    }
    if (sicak >= 100)
    {
        Lcd_Putch(yuzler + 48);             // ascii deki karþýlýðý için +48 ekliyoruz (+30H)
        Lcd_Putch(onlar + 48);              // ki lcd ye yollayabilelim
        Lcd_Putch(birler + 48);
        UARTCharPut(UART0_BASE, yuzler+48); //LCD ye gönderdiðimiz verileri uarta da göndericez ki
        UARTCharPut(UART0_BASE, onlar+48);  //textboxun içine yazdýralým
        UARTCharPut(UART0_BASE, birler+48);
    }
    else if (sicak >= 10)
    {
        Lcd_Putch(' ');                     // sayý 2 basamaklýysa baþýna bir boþluk ekle " 12" gibi
        Lcd_Putch(onlar + 48);
        Lcd_Putch(birler + 48);
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, onlar+48);
        UARTCharPut(UART0_BASE, birler+48);
    }
    else
    {
        Lcd_Putch(' ');                     // sayý tek basamaklýysa baþýna iki boþluk ekle "  5" gibi
        Lcd_Putch(' ');
        Lcd_Putch(birler + 48);
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, birler+48);
    }
    if(a==1){
        a=0;        //buton kesmesinde a=1 demiþtik burada a=1 se UART ile "BUTON" yazýsý göndericez
        UARTCharPut(UART0_BASE, ' ');
        UARTCharPut(UART0_BASE, 'B');
        UARTCharPut(UART0_BASE, 'U');
        UARTCharPut(UART0_BASE, 'T');
        UARTCharPut(UART0_BASE, 'O');
        UARTCharPut(UART0_BASE, 'N');
    }
    UARTCharPut(UART0_BASE, '\n');  // sharpdevelopta readline kullandýðýmýz için gönderilecek verinin bittiðini bu iþaret geldiðinde anlarýz
    Lcd_Putch('C');// sonuna Celcius iþareti ekle
}

void uartayar(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//uart rx-tx giriþi için porta yý kullanýcaz o yüzden hayat verdik
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//hayat verdik

    GPIOPinConfigure(GPIO_PCTL_PA0_U0RX);//GPIO_PA0_U0RX
    GPIOPinConfigure(GPIO_PA1_U0TX);//ilgili pinleri uartýn rx-tx i olarak ayarladýk
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1); //RX-TX

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);
    //baudrate 120-240-480-1200-9600-15200
    //uart.h 80 den ui32config parametreleri
    UARTEnable(UART0_BASE);// uart aktif edildi
}

void butonayar(){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//hayat verdik
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);//portf in 4. pini input olarak ayarladýk
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU); //pull up ayarlandý

    IntMasterEnable();//1. global ayar
    IntEnable(INT_GPIOF);//2.global ayar

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);//butona düþen kenar kesmesi ayarladýk
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);//pin 4den gelecek kesmeler þuandan itibaren aktif
    GPIOIntRegister(GPIO_PORTF_BASE, butonkesme);//kesme gelirse "butonkesme" fonksiyonuna gidicek

}

void butonkesme(){
    a=1;// printTemp fonk.da a=1 se "BUTON" diye bir yazý göndericez onun için butondan kesme gelirse a yý 1 yaptýk
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
}
