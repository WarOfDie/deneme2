#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "lcd.h"



void otuzhexgonder()
{
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x30 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);

}



void Lcd_init() {
    SysCtlPeripheralEnable(LCD_PORT_ENABLE);
    GPIOPinTypeGPIOOutput(LCD_PORT, 0xFF);
    SysCtlDelay(50000);
    GPIOPinWrite(LCD_PORT, RS,  0x00 );

    otuzhexgonder();
    otuzhexgonder();
    otuzhexgonder();


    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7,  0x20 );
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);


    Lcd_Komut(0x28,0);
    Lcd_Komut(0x06,0);
    Lcd_Komut(0x80,0);
    Lcd_Komut(0x0f,0);
    Lcd_Temizle();
}

void Lcd_Gotoxy(char x, char y){
    if(x==1)
        Lcd_Komut(0x80+((y-1)%16),0);
    else
        Lcd_Komut(0xC0+((y-1)%16),0);
}

void Lcd_Temizle(void){
    Lcd_Komut(0x01,0);
    SysCtlDelay(10);
}

void Lcd_Yaz( char* s){
    while(*s)
        Lcd_Komut(*s++,1);
}

//komut ise 0 , karakter ise 1
void Lcd_Komut(unsigned char c,int type) {
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0xf0) );
    if(type==1)GPIOPinWrite(LCD_PORT, RS, 0x01);
    else GPIOPinWrite(LCD_PORT, RS, 0x00);
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);


    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
    if(type==1)GPIOPinWrite(LCD_PORT, RS, 0x01);
    else GPIOPinWrite(LCD_PORT, RS, 0x00);
    GPIOPinWrite(LCD_PORT, E, 0x02);
    SysCtlDelay(10);
    GPIOPinWrite(LCD_PORT, E, 0x00);
    SysCtlDelay(50000);
}
