#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "I2C.h"  //include library for i2c driver
#include "ssd1306.h" //include display driver
unsigned counter= 0;
void init(){
	PORTK|=0xFF;
	DDRB |=(1<<7);  //D4 as output
	DDRK &= ~(1<<0);
  PORTK|=(0xFF);
}

int main(void)
{  
	init();
  
  I2C_Init();  //initialize i2c interface to display

  InitializeDisplay(); //initialize  display

  clear_display();   //use this before writing you own text

  char HexPinstate[15];
  strcpy(HexPinstate,"PINK:0X");
  int hexlen = strlen(HexPinstate);
  
  char PINbuffer[3];
  
  
  bool startcondition= false;
  
  while (1)
  {      


    PORTB ^= (1<<7); // blinking onboard led every 1 second
    _delay_ms(1000);


    // Alt det grønne text her er debug til at læse registre. Jeg lader det lige stå hvis det skal bruges igen
	  //itoa(PORTK, PORTbuffer, 10);
    
    itoa(PINK, PINbuffer, 10);
    sendStrXY(PINbuffer,1,0); // her printer jeg decimal tallet a register værdien
    
    
    //sendStrXY(PORTbuffer,0,0); 
    
    // if (firsttime == 0)
    // {
    //   strcpy(PINstatecurrent,PINbuffer);
    // }  
    for (int i = 0; i<=7;i++)
    {
      if (!(PINK & (1<<i)))
      {
        itoa(PINK, PINbuffer, 10);
        sendStrXY(PINbuffer,1,0);
        
        char switchbuffer[2];
        itoa(i+1,switchbuffer,10);
        
        sendStrXY(switchbuffer, 3,0); // Her printer jeg hvilken switch der bliver åbnet

        while(!(PINK & (1<<i)))
        {
          if (!startcondition)
          {
            itoa(PINK,PINbuffer,16);
            int slen = strlen(PINbuffer);
            for (int i = 0; i<slen;i++)
            {
              PINbuffer[i] = toupper(PINbuffer[i]);
            }
            strcat(HexPinstate,PINbuffer);
            startcondition = true;
          }
          
          sendStrXY(HexPinstate,5,0); // Her printer jeg hvilken hexa værdi registeret har for den givne switch
        }
        HexPinstate[hexlen] = '\0';
        startcondition = false;
        clear_display(); // Gør klar til næste switch
      }
    }
  }
}
