#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "lcd.h"
#include "driverlib/uart.c"
//revert deneme
uint32_t gelendeger[4];
uint32_t adcveri[4];
uint32_t gelenveri[8];
long ort,temp;
int sa1=0x30;
int sa2=0x30;
int dk1=0x30;
int dk2=0x30;
int sn1=0x30;
int sn2=0x30;
int potonbin;




void timerkesmesi()
{

    int i = 0;
        while(UARTCharsAvail(UART0_BASE))
        {
            gelenveri[i]=UARTCharGet(UART0_BASE);
            i++;

        }
    SysCtlDelay(SysCtlClockGet()/3);
    Lcd_Temizle();
    Lcd_Gotoxy(1,5);
    Lcd_Komut(gelenveri[7], 1);
    Lcd_Komut(gelenveri[6], 1);
    Lcd_Yaz(":");
    Lcd_Komut(gelenveri[4], 1);
    Lcd_Komut(gelenveri[3], 1);
    Lcd_Yaz(":");
    Lcd_Komut(gelenveri[1], 1);
    Lcd_Komut(gelenveri[0], 1);

    gelenveri[0]++;
    if(gelenveri[0]==0x39)
    {
        gelenveri[0]=0x30;
        gelenveri[1]++;
       if(gelenveri[1]==0x36)
       {
           gelenveri[1]=0x30;
           gelenveri[3]++;
           if(gelenveri[3]==0x39)
               {
               gelenveri[3]=0;
               gelenveri[4]++;
                  if(gelenveri[4]==0x36)
                  {
                      gelenveri[4]=0x30;
                      gelenveri[6]++;
                      if(gelenveri[6]==0x39)
                          {
                          gelenveri[6]=0;
                          gelenveri[7]++;
                             if(gelenveri[7]==0x36)
                             {
                                 gelenveri[7]=0x30;




                             }


                          }



                  }


               }



       }


    }
    for(i=0; i<5;i++){

                             UARTCharPut(UART0_BASE, adcveri[i]);


            }



}



void adckesme(){

   int i;
   ADCSequenceDataGet(ADC0_BASE, 2, gelendeger);
   ort=(gelendeger[0]+gelendeger[1]+gelendeger[2]+gelendeger[3])/4;

   int pot=ort*2.44;
   //12345
   int potonbin=(pot/10000) + 0x30 ;
   int potbin=(pot%10000)/1000 + 0x30 ;
   int potyüz=(pot%1000)/100 + 0x30 ;
   int poton=(pot%100)/10 + 0x30 ;
   int potbir=(pot%10) + 0x30 ;


      Lcd_Gotoxy(2, 6);
      Lcd_Komut(potonbin, 1);

            Lcd_Komut(potbin, 1);

                  Lcd_Komut(potyüz, 1);

                        Lcd_Komut(poton, 1);

                              Lcd_Komut(potbir, 1);
      ADCIntClear(ADC0_BASE, 2);
         ADCProcessorTrigger(ADC0_BASE, 2);
         adcveri[0]=potonbin;
                     adcveri[1]=potbin;
                     adcveri[2]=potyüz;
                     adcveri[3]=poton;
                     adcveri[4]=potbir;




}




int main(void)

{
       Lcd_init();
       SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); // 40mhz
       long a=SysCtlClockGet();
//     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

       IntMasterEnable();
       IntEnable(INT_ADC0SS2);

       ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);// seq 2 kullanilacak
       ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH0 );
       ADCSequenceStepConfigure(ADC0_BASE, 2, 1, ADC_CTL_CH0 );
       ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_CH0 );
       ADCSequenceStepConfigure(ADC0_BASE, 2, 3, ADC_CTL_CH0 |ADC_CTL_IE|ADC_CTL_END);
       ADCIntClear(ADC0_BASE, 2);
       ADCIntRegister(ADC0_BASE, 2, adckesme);
       ADCIntEnable(ADC0_BASE, 2);


       ADCSequenceEnable(ADC0_BASE, 2);

       ADCProcessorTrigger(ADC0_BASE, 2); // CEVRIM BASLADI


           SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
//           GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


           IntEnable(INT_TIMER0A);

           TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
           TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

           TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
           TimerIntRegister(TIMER0_BASE, TIMER_A, timerkesmesi);
           TimerEnable(TIMER0_BASE, TIMER_A);
           GPIOPinConfigure(GPIO_PCTL_PA1_U0TX);
           GPIOPinConfigure(GPIO_PCTL_PA0_U0RX);
           GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);
           UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE);

    while(1)
    {







    }


}
