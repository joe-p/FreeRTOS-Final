#define FCY 8000000UL       // instruction cycle Hrtz

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "timers.h"   

// Initializes registers to the proper values and sets up the timer and queue
void adc_init(void);

// Interrupt when AD1IF is set
// Calculates the input current from the ADC voltage. 
void __attribute__ ((__interrupt__)) _ADC1Interrupt(void);

// Callback function for the 10ms periodic timer that starts ADC sampling and conversion
void vAdcTimerCallback( TimerHandle_t xTimer );
