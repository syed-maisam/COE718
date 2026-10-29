/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher and Graphic Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H> /* LPC17xx definitions */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

 int selector();
 int selected();


extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned int counter;

extern unsigned char KOBE_pixel_data[];
extern unsigned char TESLA_pixel_data[];
extern unsigned char MUSIC_pixel_data[];


int images(int x)		//function for displaying image
{
  	/* Main Program                       */
	 //selector to see which program is user the choosing
	
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();
  LED_Init ();

  GLCD_Clear(Black);
	
  SysTick_Config(SystemCoreClock/100); 

  while(1)		//loop forever
	{
			joystick_val = get_button();

				/*******************************************************************************
				* Set draw window region                                                       *
				*   Parameter:      x:        horizontal position                              *
				*                   y:        vertical position                                *
				*                   w:        window width in pixel                            *
				*                   h:        window height in pixels                          *
				*   Return:   																																 *
				*  																																						 *	
				*   WIDTH       320           Screen Width (in pixels)           							 *	
				*	  HEIGHT      240           Screen Hight (in pixels) 												 *
				*																																							 *
				*******************************************************************************/
		
				if(x == 1){
					GLCD_DisplayString(0,0,1, "Supercar goes fast!!");
					GLCD_Bitmap(44, 55, 232, 130, TESLA_pixel_data);
					LED_On(x-1);

				}
				else if(x == 2){
					GLCD_DisplayString(0,0,1, "NBA Legend Kobe");
					GLCD_Bitmap(96, 40, 128, 160, KOBE_pixel_data);

					LED_On(x-1);
				}
				else if(x == 3){
					GLCD_DisplayString(0,0,1, "Jamming to tones");
				  GLCD_Bitmap(85, 45, 180, 135, MUSIC_pixel_data);
					LED_On(x-1);
				}
				
				GLCD_DisplayString(9,0,1, "Exit with left");

			
			if (joystick_val != joystick_prev)
			{
					if (joystick_val == KBD_LEFT)
					{
							LED_Off(x-1);
							GLCD_Clear(Black);
							return(0);
					}	
			}
	}
}