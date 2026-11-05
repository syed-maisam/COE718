// COE 718 - Lab4 - Part 1
// CMSIS-RTOS main function template

#define osObjectsPublic                     // define RTOS objects in this file
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                       // main RTOS header
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                      // LED control functions

int count = 0; // keeps track of how many times the LED has flashed
int flag  = 0; // used to control when timers restart
void delay(unsigned int); // custom delay function prototype

// Declare the three LED thread functions
void led_ThreadC (void const *argument);
void led_ThreadB (void const *argument);
void led_ThreadA (void const *argument);

// Define each thread for the RTOS
osThreadDef (led_ThreadC, osPriorityNormal, 1, 0);
osThreadDef (led_ThreadB, osPriorityNormal, 1, 0);
osThreadDef (led_ThreadA, osPriority/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define RTOS objects in this main module
#include "osObjects.h"                      // RTOS object definitions
#include "cmsis_os.h"                       // main RTOS header
#include <stdio.h>
#include <math.h>
#include "Board_LED.h"                      // LED support functions
#include "RTE_Components.h"                 // component configuration file

  
/*----------------------------------------------------------------------------
	CMSIS RTX Priority Inversion Example
	Priority Inversion = leave commented lines commented
	Priority Elevation = uncomment the 2 commented lines
	Author: Anita Tino
*----------------------------------------------------------------------------*/

// Declare thread functions
void P1 (void const *argument);
void P2 (void const *argument);
void P3 (void const *argument);

// Define threads and their priorities
osThreadDef(P1, osPriorityHigh, 1, 0);         // Thread P1 - high priority
osThreadDef(P2, osPriorityNormal, 1, 0);       // Thread P2 - normal priority
osThreadDef(P3, osPriorityBelowNormal, 1, 0);  // Thread P3 - low priority

// Thread IDs to reference each thread later
osThreadId t_main, t_P1, t_P2, t_P3;

// Simple delay loop (simulates work)
void delay(){ 
	long k, count = 0;
	for(k = 0; k < 100000; k++){
		count++; // empty loop just to waste time
	}
}

/*------------------------------------------------------
   P1: High-priority thread
   - Does some work
   - Then needs a service from P3 (low priority)
------------------------------------------------------*/
void P1 (void const *argument) {
	for (;;) 
	{
		LED_On(0);                 // Turn on LED0
		delay();                   // Simulate doing work
		
		// osThreadSetPriority(t_P3, osPriorityHigh); // Uncomment to fix priority inversion
		
		osSignalSet(t_P3, 0x01);    // Ask P3 to perform a task
		osSignalWait(0x02, osWaitForever); // Wait for P3 to finish
		
		// osThreadSetPriority(t_P3, osPriorityBelowNormal); // Uncomment to restore priority
		
		LED_On(6);                  // Indicate completion
		LED_Off(6);
	}
}

/*------------------------------------------------------
   P2: Medium-priority thread
   - Runs continuously and can delay P3’s execution
   - Causes priority inversion problem
------------------------------------------------------*/
void P2 (void const *argument) {
	for (;;) {
		LED_On(1);   // Turn on LED1
		LED_Off(1);  // Turn off LED1
	}
}

/*------------------------------------------------------
   P3: Low-priority thread
   - Performs a critical section that P1 needs
   - If P2 runs first, P3 gets delayed (priority inversion)
------------------------------------------------------*/
void P3 (void const *argument) {
	for (;;) {
		delay();                        // Do some work
		osSignalWait(0x01, osWaitForever); // Wait for signal from P1
		LED_Off(0);                     // Perform "critical" action
		osSignalSet(t_P1, 0x02);        // Notify P1 that it’s done
	}
}

/*------------------------------------------------------
   main: Initializes and starts the threads
------------------------------------------------------*/
int main(void)
{    
	osKernelInitialize ();     // Initialize RTOS kernel
	LED_Initialize();          // Initialize LEDs on the board
	
	t_main = osThreadGetId (); // Get ID of the main thread
	osThreadSetPriority(t_main, osPriorityHigh); // Set main as high priority temporarily
	
	// Create threads in a staggered order
	t_P3 = osThreadCreate(osThread(P3), NULL);  // Create low-priority thread first
	osDelay(5);
	t_P2 = osThreadCreate(osThread(P2), NULL);  // Then normal-priority thread
	osDelay(5);
	t_P1 = osThreadCreate(osThread(P1), NULL);  // Finally high-priority thread
	
	osThreadTerminate(t_main); // Stop the main thread (no longer needed)
	osKernelStart ();          // Start the RTOS scheduler — threads start running
	
	for (;;) {}                // Infinite loop (should never reach here)
}
Normal, 1, 0);


	//Virtual Timer declaration and callback method

// Thread IDs for later reference
osThreadId T_led_ID1;
osThreadId T_led_ID2;	
osThreadId T_led_ID3;	

// Callback function that runs when a timer expires
void callback(void const *param){
	count = count + 1;  // simply increase the count each time the timer triggers
}

// Define three software timers that use the same callback
osTimerDef(timer0_handle, callback);
osTimerDef(timer1_handle, callback);
osTimerDef(timer2_handle, callback);


  
  // Thread C - Flash LED3, then signal Thread B when done

void led_ThreadC (void const *argument) {
	
	osTimerId timer_0 = osTimerCreate(osTimer(timer0_handle), osTimerPeriodic, NULL); // create timer
	osTimerStart(timer_0, 500); // start timer with 500ms interval
	
	for (;;) {
		LED_On(3);  // turn on LED3
		Delay(10);  // short delay
		LED_Off(3); // turn off LED3
		
		osSignalSet(T_led_ID1,0x03); // signal this thread to continue
		
		if (count == 4){ // after 4 timer callbacks
			count = 0;  // reset count
			flag = 1;   // set flag to restart timer later
			
			osSignalSet(T_led_ID2,0x01); // tell next thread (Thread B) to run
			osTimerStop(timer_0); // stop this thread’s timer
			osSignalWait(0x03, osWaitForever); // wait until signaled again
		}
		
		osSignalWait(0x03, osWaitForever); // wait before next cycle
		
		if (flag == 1){ // restart timer only once after switching
			osTimerStart(timer_0, 500);
			flag = 0;
		}
	}
}



  //Thread B - Flash LED4, then signal Thread A when done
  
void led_ThreadB (void const *argument) {
	osTimerId timer_1 = osTimerCreate(osTimer(timer1_handle), osTimerPeriodic, NULL);
	osTimerStart(timer_1, 1000); // timer with 1-second interval
	
	for (;;) {
		LED_On(4);  // turn on LED4
		Delay(10);  // short delay
		LED_Off(4); // turn off LED4
		
		osSignalSet(T_led_ID2,0x01); // continue thread
		
		if (count == 4){ // after 4 timer callbacks
			count = 0;
			flag = 1;
			
			osTimerStop(timer_1); // stop timer
			osSignalSet(T_led_ID3,0x02); // signal Thread A
			osSignalWait(0x01, osWaitForever); // wait to be reactivated
		}
		
		osSignalWait(0x01, osWaitForever); // normal wait
		
		if (flag == 1){
			osTimerStart(timer_1, 1000); // restart timer
			flag = 0;
		}
	}
}


  //Thread A - Flash LED5, then signal Thread C when done
  
  
void led_ThreadA (void const *argument){
	osTimerId timer_2 = osTimerCreate(osTimer(timer2_handle), osTimerPeriodic, NULL);
	osTimerStart(timer_2, 2000); // timer with 2-second interval
	
	for (;;) {
		LED_On(5);  // turn on LED5
		Delay(10);  // short delay
		LED_Off(5); // turn off LED5
		
		osSignalSet(T_led_ID3,0x02); // continue thread
		
		if (count == 2){ // after 2 timer callbacks (shorter loop)
			count = 0;
			flag = 1;
			
			osTimerStop(timer_2); // stop timer
			osSignalSet(T_led_ID1,0x03); // signal Thread C to start
			osSignalWait(0x02, osWaitForever); // wait for next activation
		}
		
		osSignalWait(0x02, osWaitForever);
		
		if (flag == 1){
			osTimerStart(timer_2, 2000); // restart timer
			flag = 0;
		}
	}
}


  //Main function - start kernel and create threads

int main (void) {
	osKernelInitialize (); 	// initialize RTOS kernel
  LED_Initialize ();	    // initialize LEDs on the board

	// create the three LED threads
	T_led_ID1 = osThreadCreate(osThread(led_ThreadC), NULL);
	T_led_ID2 = osThreadCreate(osThread(led_ThreadB), NULL);
  T_led_ID3 = osThreadCreate(osThread(led_ThreadA), NULL);
	
	osKernelStart ();        // start RTOS scheduler (threads begin running)
	osDelay(osWaitForever);  // keep main alive forever
	
	for (;;); // infinite loop (never reached)
}


  //Custom busy-wait delay function (simple nested loop)
  
  
void delay (unsigned int value){ 
	unsigned int count1 = 0;
	unsigned int count2 = 0;
	
	for (count1 = 0; count1 < value; count1++){
		for (count2 = 0; count2 < count1; count2++){
			// empty loop for delay
		}
	}	
}
