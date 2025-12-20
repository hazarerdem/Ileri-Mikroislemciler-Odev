#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"

int main(void) {
 SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    Lcd_init();
    Lcd_Goto(1,2);                  //lcd nin 1.satýr 2.sütununa gidip "DOGUKAN HAZAR" yaz
    Lcd_Puts("DOGUKAN HAZAR");
    Lcd_Goto(2,0);                  //lcd nin 2.satýr 0.sütununa gidip "ERDEM B220100030" yaz
    Lcd_Puts("ERDEM B220100030");

    while (1) {
//        Lcd_Komut(0x18);          // sola kaydýr
//        SysCtlDelay(SysCtlClockGet()/8); //kaymanýn hýzý
    }
}
