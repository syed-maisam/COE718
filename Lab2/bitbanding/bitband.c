#include "LPC17xx.h"
#include <stdio.h>
#include <stdbool.h>
#include "GLCD.h"
#include "LED.h" // Assumes LED.h contains the prototype for LED_Init()
#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0                                        /* Uncomment to use the LCD */

//------- ITM Stimulus Port definitions for printf ------------------- //
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

volatile unsigned long msTicks = 0; // Variable for SysTick Interrupt

// SysTick Handler (Interrupt Service Routine for the SysTick interrupt)
void SysTick_Handler(void) {
    msTicks++;
}

// Initialize SysTick
void SysTick_Initialize(unsigned long ticks) {
    SysTick->LOAD = (ticks & 0xFFFFFF) - 1;  // Set reload register
    SysTick->VAL   = 0;                                         // Reset the SysTick counter value
    SysTick->CTRL  = 0x07;                                     // Enable SysTick IRQ and SysTick Timer
}

// Delay using SysTick timer
void delay_ms(unsigned long delayTicks) {
    unsigned long currentTicks;
    currentTicks = msTicks; // Save current ticks
    while ((msTicks - currentTicks) < delayTicks); // Wait until required time passes
}
//------------------------------------------------------------------- //

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y)     ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))

volatile unsigned long * bit1;
volatile unsigned long * bit2;
//#define ADCR_Bit24   (*((volatile unsigned long *)0x42680060))
#define FIOPIN_Bit28 (*((volatile unsigned long *)0x233806F0))
#define FIOPIN_Bit02 (*((volatile unsigned long *)0x23380A88))
    
#define BASE_DELAY             1000
#define BASE_ADD                 200
int currentShift = 0;
int shiftDirection = 1;
char buffer[10];
char buffer1[10];
char barshift[15] = "1000 + 200x";

int main(void){
    SysTick_Initialize(100000); 
    LED_Init(); // This is where warning #223-D occurs if LED_Init is not prototyped in LED.h
    
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
//clear this part out for now
GLCD_Clear(White);                         /* Clear graphical LCD display   */
GLCD_SetBackColor(Blue);
GLCD_SetTextColor(Yellow);
GLCD_DisplayString(0, 0, __FI, (unsigned char*)"COE718 Lab2 (Maisam)  "); // Cast added for completeness
GLCD_SetTextColor(White);

GLCD_SetBackColor(White);
GLCD_SetTextColor(Blue);

// char base_buffer[10]; // REMOVED: Warning #177-D resolved
GLCD_DisplayString(4, 0, __FI, (unsigned char*)"      Method:        "); // Cast added for completeness
#endif
//clear this part out for now
    bool led_switch = false;

    while (1) {
        //Mask mode
        printf("\nMask Method\n");    
#ifdef __USE_LCD
        GLCD_DisplayString(6, 6, __FI, (unsigned char *)"Mask              ");
#endif
        if (led_switch) {
            LPC_GPIO1->FIOPIN &=  ~(1UL<< 28);
            LPC_GPIO2->FIOPIN &=  ~(1UL<< 2);    
            led_switch = false;
            delay_ms(BASE_DELAY + (BASE_ADD*currentShift));
        }
        else {
            LPC_GPIO1->FIOPIN |=  (1UL<< 28);
            LPC_GPIO2->FIOPIN |=  (1UL<< 2);    
            led_switch = true;        
            delay_ms(BASE_DELAY + (BASE_ADD*currentShift));
        }
        
        //Function mode
        printf("\nFunction Method\n");
#ifdef __USE_LCD
        GLCD_DisplayString(6, 6, __FI, (unsigned char *)"Function    ");
#endif
        bit1 = &BitBand(&LPC_GPIO1->FIOPIN, 28);
        bit2 = &BitBand(&LPC_GPIO2->FIOPIN, 2);
        led_switch ? (*bit1 = 0) : (*bit1 = 1);
        led_switch ? (*bit2 = 0) : (*bit2 = 1);
        led_switch ? (led_switch = false) : (led_switch = true);
        delay_ms(BASE_DELAY + (BASE_ADD*currentShift));

        //Bit band mode
        printf("\nDirect Bit Banding Method\n");
#ifdef __USE_LCD
        GLCD_DisplayString(6, 6, __FI, (unsigned char *)"Bit Banding    ");
#endif
        if (led_switch) {
            FIOPIN_Bit28 = 0;
            FIOPIN_Bit02 = 0;
            led_switch = false;
            delay_ms(BASE_DELAY + (BASE_ADD*currentShift));
        }
        else {
            FIOPIN_Bit28 = 1;
            FIOPIN_Bit02 = 1;
            led_switch = true;
            delay_ms(BASE_DELAY + (BASE_ADD*currentShift));
        }
    }
}