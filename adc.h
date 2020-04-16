#define FCY 8000000UL       // instruction cycle Hrtz

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "timers.h"   

void adc_init(void);

void adc_sample(void);

void __attribute__ ((__interrupt__)) _ADC1Interrupt(void);

void vAdcTimerCallback( TimerHandle_t xTimer );