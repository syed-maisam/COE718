#include <stdio.h>                                 
#include <ctype.h>                   
#include <string.h>    
#include <math.h>
#include "cmsis_os.h"
#include "RTL.H"  
#include "LPC17xx.H"           
#include "GLCD.h"

#define __FI        1                   // Font index for LCD display
#define __USE_LCD   0                   // Uncomment for DEMO mode

#define PI 3.142                        

double factor;                          

// Thread function declarations
void threadA (void const *argument); 
void threadB (void const *argument); 
void threadC (void const *argument); 
void threadD (void const *argument); 
void threadE (void const *argument); 

osThreadId id_Thread_A, id_Thread_B, id_Thread_C, id_Thread_D, id_Thread_E;

osThreadDef (threadA, osPriorityAboveNormal, 1, 0);   // Priority level #2
osThreadDef (threadB, osPriorityBelowNormal, 1, 0);   // Priority level #3
osThreadDef (threadC, osPriorityHigh, 1, 0);          // Priority level #1 (highest)
osThreadDef (threadD, osPriorityAboveNormal, 1, 0);   // Priority level #2
osThreadDef (threadE, osPriorityBelowNormal, 1, 0);   // Priority level #3

//  initialize and create all threads
int Init_Thread (void) {
  id_Thread_A = osThreadCreate (osThread(threadA), NULL);    
  id_Thread_B = osThreadCreate (osThread(threadB), NULL);
  id_Thread_C = osThreadCreate (osThread(threadC), NULL);
  id_Thread_D = osThreadCreate (osThread(threadD), NULL);
  id_Thread_E = osThreadCreate (osThread(threadE), NULL);
  
  return(0);
}


// Thread A: Performs addition operation in a loop
__task void threadA (void const *arg) {
  char out[20];
  int A = 0;
  int x = 0;
  for (x = 0; x < 257; x++) { 
    A = A + (x + (x + 2));                     
    os_tsk_pass();                             // Pass control to next ready task
  }
    //printf("Thread A done. A = %d\n", A);

}

// Thread B: Calculates exponential series using factorial
__task void threadB (void const *arg) {
  float B = 0;
  char out[20];
  int i = 0;
  factor = 1;
  for (i = 1; i < 17; i++) {
    factor = factor * i;                       // Calculate factorial
    B = B + pow(2, i) / factor;                // Add term to series
    os_tsk_pass();                             // Pass control to next ready task
  }
   // printf("Thread B done. B = %.3f\n", B);

}

// Thread C: Performs division and addition operations
__task void threadC (void const *arg) {
  float C = 0;
  char out[20];
  int n = 0;
  for (n = 1; n < 17; n++) {
    C = C + (n + 1) / n;                       // Simple arithmetic operation
  }
   // printf("Thread C done. C = %.3f\n", C);

}

// Thread D: Calculates factorial-based power series
__task void threadD (void const *arg) {
  float D = 0;
  char out[20];
  int m = 0;
  factor = 1;
  for (m = 0; m < 6; m++) {
    factor = factor * m;                       // Calculate factorial
    if (factor == 0) {
      factor = 1;                              // Avoid division by zero
    } else {
      os_tsk_pass();                           // Pass control to next ready task
      D = D + pow(5, m) / (double)factor;      // Add term to power series
    }
    
  }
   //printf("Thread D done. D = %.3f\n", D);
}

// Thread E: Calculates area-like values using p and radius
__task void threadE (void const *arg) {
  int E = 0;
  char out[20];
  int p = 0;
  int radius = 1;
  for (p = 1; p < 13; p++) {
    E = E + p * PI * (pow(radius, 2));         // Calculate area * iteration
    os_tsk_pass();                             // Pass control to next ready task
  }
    //printf("Thread E done. E = %.3f\n", E);

}
