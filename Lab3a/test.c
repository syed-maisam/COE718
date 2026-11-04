#include <stdio.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "LPC17xx.h"                      
#include "GLCD.h"
#include "LED.h"
#define __FI        1 
#define __USE_LCD   1                       /* 1 = on, 0 = off            */

/*----------------- ITM Stimulus Port definitions for printf -----------------*/
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000



struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;

/* Redirect printf to ITM port 0 */
int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);   // wait until ready
    ITM_Port8(0) = ch;            // send character
  }
  return ch;
}



/*----------------------------------------------------------------------------
 *      Sample threads
 *---------------------------------------------------------------------------*/
 unsigned int counta=0;
unsigned int countb=0;
unsigned int countc=0;
 

  
void Thread1 (void const *argument); // thread function
void Thread2 (void const *argument); // thread function
void Thread3 (void const *argument); // thread function

osThreadId tid_Thread; // thread id
osThreadDef (Thread1, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid2_Thread; // thread id
osThreadDef (Thread2, osPriorityNormal, 1, 0);                   // thread object

osThreadId tid3_Thread; // thread id
osThreadDef (Thread3, osPriorityNormal, 1, 0);                   // thread object

volatile int i;

int Init_Thread (void) {
  LED_Init();
  tid_Thread = osThreadCreate (osThread(Thread1), NULL);
	tid2_Thread = osThreadCreate (osThread(Thread2), NULL);
	tid3_Thread = osThreadCreate (osThread(Thread3), NULL);
  if(!tid_Thread) return(-1);
  
  return(0);
}



void Thread3(void const *argument) {
    for (;;) {
      LED_On(0);      
      LED_On(1);
      LED_On(2);
      LED_On(3);
      LED_On(4);
			#if (__USE_LCD == 1)
			GLCD_Init();                          /* LCD is on         */
			GLCD_DisplayString(2, 0, __FI, "      Thread3.c              ");
			#endif
      countc++;
      printf("Thread3 running, LCD & LED - ON\n", countc);
		}
}


void Thread2 (void const *argument) {
    for (;;) {
		 LED_Off(0); 
		 LED_Off(1);
		 LED_Off(2);
		 LED_Off(3);
		 LED_Off(4);				
      countb++;
		  printf("Thread2 running, LCD - ON\n", countb);
    }                                        
} 

void Thread1 (void const *argument) {
	 for (;;) {
		 LED_On(0); 
		 LED_Off(1);
		 LED_Off(2);
		 LED_Off(3);
		 LED_Off(4);
		 counta++;
		 printf("Thread1 running, P1.38 - ON\n");
    }	 
}
