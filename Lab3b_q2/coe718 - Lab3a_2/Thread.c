#include <stdio.h>                                 
#include <ctype.h>                   
#include <string.h>    
#include <math.h>
#include "cmsis_os.h" 
#include "LPC17xx.H"           

#define ADDRESS(x) (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))

#define GPIO1_LED31 (*((volatile unsigned long*)0x233806FC))
#define GPIO2_LED2 (*((volatile unsigned long *) 0x23380A88))
#define __FI        1                   

// Global variables for storing results from each thread
double factor;
double var_A;
double var_B;
double var_C;
double var_D;
double var_E;

int myExponentialCalc(int, int);
void delay (unsigned int);
volatile unsigned long * GPIO2_LED4 ;

// Thread function declarations
void threadA (void const *argument); 																					
void threadB (void const *argument); 
void threadC (void const *argument); 
void threadD (void const *argument); 
void threadE (void const *argument); 

// Thread IDs
osThreadId id_Thread_A, id_Thread_B, id_Thread_C, id_Thread_D, id_Thread_E; 	

// Define threads and assign their priorities
osThreadDef (threadA, osPriorityAboveNormal, 1, 0); 	// Priority #2
osThreadDef (threadB, osPriorityBelowNormal, 1, 0); 	// Priority #3
osThreadDef (threadC, osPriorityHigh, 1, 0); 				// Priority #1 (highest)
osThreadDef (threadD, osPriorityAboveNormal, 1, 0); 	// Priority #2
osThreadDef (threadE, osPriorityBelowNormal, 1, 0); 	// Priority #3

// Function to initialize and create all threads
int Init_Thread (void) {

 id_Thread_A = osThreadCreate (osThread(threadA), NULL);   	// Create threads ABCDE
 id_Thread_B = osThreadCreate (osThread(threadB), NULL);   	
 id_Thread_C = osThreadCreate (osThread(threadC), NULL);   	
 id_Thread_D = osThreadCreate (osThread(threadD), NULL);   	
 id_Thread_E = osThreadCreate (osThread(threadE), NULL);   	
 
 if(!id_Thread_A) return(-1);	
	
  return(0);
}

// ------------------------- Thread A -------------------------
void threadA (void const *arg) {
 double x =0;
 int i = 0;
 
 // Compute A = sum[x + (x + 2)] for x = 0 to 256
 for ( i=0; i<257; i++){ 
  x = x + (i + (i+2));
  var_A = x;	// Store result in global variable
 }
 
 osDelay(1);	// Short delay 
}

// ------------------------- Thread B -------------------------
void threadB (void const *arg) {
 double x =0;
 int i;
 int factor = 1;
	
 // Compute B = sum (2^n / n!) for n = 1 to 16
 for( i = 1; i<17; i++){
  factor = factor*i;	// Compute factorial
  x = x + ((double)(myExponentialCalc(2,i))/factor);	// Add term to sum
	var_B = x;	// Store result in global variable
	osDelay(1);	
 }
}

// ------------------------- Thread C -------------------------
void threadC (void const *arg) {
 double x =0;
 int n=0;

 // Compute C = sum((n+1)/n) for n = 1 to 16
 for ( n=1; n<17; n++){
  x = x + (n+1)/n;
  var_C = x;	
 }
}

// ------------------------- Thread D -------------------------
void threadD (void const *arg) {
 double x=0;
 int m=0;
 factor=1;

 // Compute D = 1 + 5/1! + 5^2/2! + ... + 5^5/5!
 for ( m=0; m<6; m++){
  factor = factor*m;	// Compute factorial
  if(factor == 0){
   factor=1;	// Avoid division by zero 
  }
  else{
   osDelay(1);	
	 x = x + ((double)(myExponentialCalc(5, m)))/(double)factor;	// Add term to sum
   var_D = x;	
	}
 }
}

// ------------------------- Thread E -------------------------
void threadE(void const *arg) {
  double x=0;
	int p=0;
	int radius=1;

 // Compute E = pr^2 + 2pr^2 + ... + 12pr^2
 for (p=1; p<13; p++){
  x = x + (3.14)*((double)(myExponentialCalc(radius,2)));	// Add pr^2 
  var_E = x;	
	osDelay(1);	
 }
}

// ------------------------- Helper Functions -------------------------

// Function to compute exponential
int myExponentialCalc(int x, int n)
{
	int i;
	int number = 1; 
	
	for (i = 0; i < n; i++)
		number *=x;	// Multiply x n times
		
	return(number);	
}

void delay (unsigned int value){
	unsigned int count1 = 0;
	unsigned int count2 = 0;
	
	for (count1 = 0; count1 < value; count1++){
		for (count2 = 0; count2 < count1; count2++){
			
		}
	}	
}	