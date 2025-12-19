/*
 * lcd.h
 *
 *  Created on: 4 Kas 2025
 *      Author: 90505
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LCDPORTENABLE  SYSCTL_PERIPH_GPIOB
#define LCDPORT        GPIO_PORTB_BASE

#define RS GPIO_PIN_0
#define E  GPIO_PIN_1
#define D4 GPIO_PIN_2
#define D5 GPIO_PIN_3
#define D6 GPIO_PIN_4
#define D7 GPIO_PIN_5

void Lcd_init(void);
void Lcd_Komut(unsigned char);
void Lcd_Putch(unsigned char);
void Lcd_Puts(char*s);
void Lcd_Goto(char,char);
void Lcd_Temizle(void);

#endif



