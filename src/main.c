/*
 * opg1_ny_F22.c
 *
 * Created: 01-09-2020 14:34:13
 *updated 7-02-22
 * Author : osch
 */ 

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
	DDRG |=0b00100000;  //D4 as output
	DDRK &= ~(1<<0);
  PORTK|=(0xFF);
}

int main(void)
{  
	init();
  
  I2C_Init();  //initialize i2c interface to display

  InitializeDisplay(); //initialize  display
  clear_display();   //use this before writing you own text
  //DDRH |= (1<<5);
  //PORTH |= (1<<5);
  char HexPinstate[25];
  strcpy(HexPinstate,"PINK:0X");
  int hexlen = strlen(HexPinstate);
  
  char PINbuffer[8];
  
  
  bool startcondition= false;
  
  while (1)
  {      
    // Alt det grønne text her er debug til at læse registre. Jeg lader det lige stå hvis det skal bruges igen
	  //itoa(PORTK, PORTbuffer, 10);
    itoa(PINK, PINbuffer, 10);
    sendStrXY(PINbuffer,1,0);
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
        
        sendStrXY(switchbuffer, 3,0);
        //registerstate =true;
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
          
          sendStrXY(HexPinstate,5,0);
        }
        HexPinstate[hexlen] = '\0';
        startcondition = false;
        clear_display();
      }
    }
  }
}
