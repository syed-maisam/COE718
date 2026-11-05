/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *   for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Anita Tino
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "Blinky.h"
#include "LPC17xx.h"                      
#include "GLCD.h"
#include "LED.h"
#include "Board_ADC.h"
#include "KBD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0 /* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

char text[10];
char text_l[10];

static volatile uint16_t AD_dbg;

uint16_t ADC_last;                      // Last converted value
/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  int32_t  res;
  uint32_t AD_sum   = 0U;
  uint32_t AD_cnt   = 0U;
  uint32_t AD_value = 0U;
  uint32_t AD_print = 0U;
int32_t key;

  LED_Init();                                /* LED Initialization            */
  ADC_Initialize();                                /* ADC Initialization            */
KBD_Init();

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demooo   ");
GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, "  Turn pot for LEDs ");
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(5, 0, __FI, "AD value:            ");
GLCD_DisplayString(8, 0, __FI, "Joystick:            ");

#endif

  //SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */

    /* AD converter input                                                     */
    // AD converter input
    res = ADC_GetValue();
key = get_button();
    if (res != -1) {                     // If conversion has finished
      ADC_last = (uint16_t)res;
     
      AD_sum += ADC_last;                // Add AD value to sum
      if (++AD_cnt == 16U) {             // average over 16 values
        AD_cnt = 0U;
        AD_value = AD_sum >> 4;          // average devided by 16
        AD_sum = 0U;
      }
    }

    if (AD_value != AD_print) {
      AD_print = AD_value;               // Get unscaled value for printout
      AD_dbg   = (uint16_t)AD_value;

      sprintf(text, "0x%04X", AD_value); // format text for print out
   

#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(6,  9, __FI,  (unsigned char *)text);
GLCD_SetTextColor(Green);
      GLCD_Bargraph (144, 7*24, 176, 20, (AD_value >> 2)); /* max bargraph is 10 bit */
#endif
    }

    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;

      printf("AD value: %s\r\n", text);
    }

switch (key) {
    case KBD_SELECT:
        printf("Joystick center\n");
        LED_Out(0);               // Turn off all LEDs
        LED_On(4);                // Turn on LED 4 for SELECT
#ifdef __USE_LCD
        GLCD_DisplayString(8, 0, __FI, "Joystick: S           ");
#endif
        break;

    case KBD_DOWN:
        printf("Joystick down\n");
        LED_Out(0);
        LED_On(1);                // Turn on LED 1 for DOWN
#ifdef __USE_LCD
        GLCD_DisplayString(8, 0, __FI, "Joystick: D           ");
#endif
        break;

    case KBD_LEFT:
        printf("Joystick left\n");
        LED_Out(0);
        LED_On(2);                // Turn on LED 2 for LEFT
#ifdef __USE_LCD
        GLCD_DisplayString(8, 0, __FI, "Joystick: L           ");
#endif
        break;

    case KBD_RIGHT:
        printf("Joystick right\n");
        LED_Out(0);
        LED_On(3);                // Turn on LED 3 for RIGHT
#ifdef __USE_LCD
        GLCD_DisplayString(8, 0, __FI, "Joystick: R           ");
#endif
        break;

    case KBD_UP:
        printf("Joystick up\n");
        LED_Out(0);
        LED_On(0);                // Turn on LED 0 for UP
#ifdef __USE_LCD
        GLCD_DisplayString(8, 0, __FI, "Joystick: U           ");
#endif
        break;

    default:
        printf("Unknown key: %d\n", key);
        LED_Out(0);               // Turn off all LEDs if no valid input
        break;
}

}

  }