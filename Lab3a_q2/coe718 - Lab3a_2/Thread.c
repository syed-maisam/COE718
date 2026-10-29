
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "osObjects.h"                      // RTOS object definitions
#include "LPC17xx.h"
#include <stdio.h>
#include "LED.h"
#include "GLCD.h"
#include <string.h>

#define __USE_LCD 0
#define __FI 1

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
	
volatile unsigned long * bit;
int a = 0;
int t = 1;
int r = 0;
/*----------------------------------------------------------------------------
 *      Sample threads
 *---------------------------------------------------------------------------*/
osMutexDef(logger_mutex_object);
osMutexId logger_mutex;

volatile unsigned int memoryAccess_counter = 0;
volatile unsigned int cpuManagement_counter = 0;
volatile unsigned int appInterface_counter = 0;
volatile unsigned int deviceManagement_counter = 0;
volatile unsigned int userInterface_counter = 0;
char logger[150];


//timer function(Aleternative for an OSdelay())
void delay(int time) {
	int x = 0;
	while (x++ < (time*10000)) {}
}
  
void memoryThread (void const *argument); // thread function
void cpuThread (void const *argument); // thread function
void applicationThread (void const *argument); // thread function
void deviceThread (void const *argument); // thread function
void userThread (void const *argument); // thread function


osThreadId tid_Thread; // thread id
osThreadDef (memoryThread, osPriorityHigh, 1, 0);   // thread object

osThreadId tid2_Thread; // thread id
osThreadDef (cpuThread, osPriorityHigh, 1, 0);   // thread object

osThreadId tid3_Thread; // thread id
osThreadDef (applicationThread, osPriorityAboveNormal, 1, 0); 

osThreadId tid4_Thread; // thread id
osThreadDef (deviceThread, osPriorityAboveNormal, 1, 0); 

osThreadId tid5_Thread; // thread id
osThreadDef (userThread, osPriorityNormal, 1, 0); 



int Init_Thread (void) {
  logger_mutex = osMutexCreate(osMutex(logger_mutex_object));
  tid_Thread = osThreadCreate (osThread(memoryThread), NULL);
	tid2_Thread = osThreadCreate (osThread(cpuThread), NULL);
  tid3_Thread = osThreadCreate (osThread(applicationThread), NULL);
  tid4_Thread = osThreadCreate (osThread(deviceThread), NULL);
  tid5_Thread = osThreadCreate (osThread(userThread), NULL);
	
	if(!tid_Thread) return(-1);
	if(!tid2_Thread) return(-1);
	if(!tid3_Thread) return(-1);
	if(!tid4_Thread) return(-1);
	if(!tid5_Thread) return(-1);
  
  return(0);
}

void userThread (void const *argument) {
	LED_Off(3);
	#ifdef __USE_LCD
	GLCD_DisplayString(6, 0, __FI, (unsigned char*)"Thread 5 - User Interface:    ");
	#endif
	LED_On(4);
	
	//osDelay(4000);
	userInterface_counter++;
	osDelay(1);
	osThreadTerminate(tid5_Thread);
}


void deviceThread (void const *argument) {
	osSignalWait(0x03, osWaitForever);
	
	LED_Off(2);
	#ifdef __USE_LCD
	GLCD_DisplayString(6, 0, __FI, (unsigned char*)"Thread 4 - Device Management:    ");
	#endif
	LED_On(3);
	
	osMutexWait(logger_mutex, osWaitForever);
	strcat(logger, "This is the final partial message from the Device Management");
	osSignalSet(tid3_Thread, 0x04);
	osMutexRelease(logger_mutex);
	
	//osDelay(4000);
	osThreadYield();
	
	deviceManagement_counter++;
	osDelay(1);
	osThreadTerminate(tid4_Thread);
}

void applicationThread (void const *argument) {
	LED_Off(1);
	#ifdef __USE_LCD
	GLCD_DisplayString(6, 0, __FI, (unsigned char*)"Thread 3 - Application Interface:    ");
	#endif
	LED_On(2);
	
	osMutexWait(logger_mutex, osWaitForever);
	sprintf(logger, "This is the partial message of the Application Interface");
	osSignalSet(tid4_Thread, 0x03);
	osMutexRelease(logger_mutex);
	
	//osDelay(4000);
	osThreadYield();
	osSignalWait(0x04, osWaitForever);
	
	appInterface_counter++;
	osDelay(1);
	osThreadTerminate(tid3_Thread);
}

void cpuThread (void const *argument) {
	cpuManagement_counter++;
	LED_Off(0);
	#ifdef __USE_LCD
	GLCD_DisplayString(6, 0, __FI, (unsigned char*)"Thread 2 - CPU Management:    ");
	#endif
	LED_On(1);
	
	for(t=1; t<4; t++){	
		  (r%2==0) ? (r+=3) : (r*=2); //Conditional Execution line
			/*if(r%2==0){
				r+=3;
			}
			else{
				r*=2;
			}*/
			a = r + (t * 2); //Barrel shifter scenario
	}
	//osDelay(4000);
	osSignalSet(tid_Thread, 0x02);
	osThreadTerminate(tid2_Thread);
}

void memoryThread (void const *argument) {
	memoryAccess_counter++;
	LED_Off(4);
	#ifdef __USE_LCD
	GLCD_DisplayString(6, 0, __FI, (unsigned char*)"Thread 1 - Memory Management:    ");
	#endif
	LED_On(0);
	
	bit = &BitBand(&LPC_GPIO2->FIOPIN, 28);
	//osDelay(4000);
	osThreadYield();
	osSignalWait(0x02, osWaitForever);
	osDelay(1);
	osThreadTerminate(tid_Thread);
}
