/*----------------------------------------------------------------------------
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
