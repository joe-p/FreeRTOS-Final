#include <xc.h>
#include <libpic30.h>
#include <math.h>

#define FCY 8000000UL       // instruction cycle Hrtz
#define ADCRES 12

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"   
#include "queue.h"
#include "dac.h"


void adc_init(void);

void adc_sample(void);

void __attribute__ ((__interrupt__)) _ADC1Interrupt(void);

void vAdcTimerCallback( TimerHandle_t xTimer );