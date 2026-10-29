
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "tic_tac_toe.h"

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
 
 extern int tic_tac_toe (void);

 
 void game_menu(){ 
	GLCD_SetBackColor(Black);
  GLCD_SetTextColor(White);
	GLCD_DisplayString (0, 4, 1, "Game Menu");
	GLCD_DisplayString (1, 0, 1, "-------------------------");
	GLCD_DisplayString (8, 0, 1, " Rules: Connect 3");
	GLCD_DisplayString (9, 1, 1, " in a row to win!!"); 
 }
 
int start_game (void) 
{  	/* Main Program                       */
	 //selector to see which program is user the choosing
	
	int selector = 0;
  int selected = 0;
	 					
	int joystick_val = 0;   //track the current joystick value
	int joystick_prev = 0;  //track the previous value for the joystick
	
	KBD_Init();
  LED_Init ();
  GLCD_Clear(Black);
	game_menu();
	
  SysTick_Config(SystemCoreClock/100); 


  while(1)		//loop forever
	{
			joystick_val = get_button();	
			
			if (joystick_val != joystick_prev)
			{
					if (joystick_val == KBD_DOWN)
					{
						if (selector >= 2){
								selector = 2;
						}
						else{
								selector +=1;
						}
					}
					else if (joystick_val == KBD_UP)
					{
						if (selector <= 1){
								selector = 1;
						}
						else{
								selector -=1;
						}
					}
				else if(joystick_val == KBD_RIGHT)
				{
						if (selector == 1)
						{
							tic_tac_toe();
							game_menu();

						}
						else if (selector == 2)
						{
							GLCD_Clear(Black);
							return(0);
						}
				}
				joystick_prev = joystick_val;	
			}
			
			if(selected == 0){
				if (selector == 1)
				{
						GLCD_SetBackColor(DarkGreen);
						GLCD_SetTextColor(White);	
						GLCD_DisplayString(5,0,1, "--> Start Game");
						GLCD_SetBackColor(Black);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(6,0,1, "    Return to Home");
						
				}
				else if(selector == 2)
				{
						GLCD_SetBackColor(Black);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(5,0,1, "    Start Game");
						GLCD_SetBackColor(DarkGreen);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(6,0,1, "--> Return to Home");

				}
				else
				{
						GLCD_SetBackColor(Black);
						GLCD_SetTextColor(White);
						GLCD_DisplayString(5,0,1, "    Start Game");
						GLCD_DisplayString(6,0,1, "    Return to Home");
				}
			}
	}
}