#include "adc.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"   
#include "queue.h"

// xAdcTimer is the periodic timer that starts sampling every 10ms
TimerHandle_t xAdcTimer;

// xAdcQueue is the queue that the calculated milliamps from the ADC voltage
// are sent to
QueueHandle_t  xAdcQueue;

// 1ms delay used between the start of sampling and the start of conversion
const TickType_t xSampleDelay = 1 / portTICK_PERIOD_MS;
 
 // input_counter and inputs and used to simulate input sampled by the ADC
int input_counter = 0;
const float inputs[11] = { 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5 };
 
// Callback function for the 10ms periodic timer that starts ADC sampling and conversion
void vAdcTimerCallback( TimerHandle_t xTimer )
{
    AD1CON1bits.SAMP = 1; // Start sampling the input
    vTaskDelay(xSampleDelay); // Delay 1ms
    AD1CON1bits.SAMP = 0; // End A/D sampling and start conversion}
}

// Initializes registers to the proper values and sets up the timer and queue
void adc_init(void) {
     AD1CON1 = 0x2200; // Configure sample clock source
    // and conversion trigger mode.
    // Unsigned Fraction format (FORM<1:0>=10),
    // Manual conversion trigger (SSRC<3:0>=0000),
    // Manual start of sampling (ASAM=0),
    // No operation in Idle mode (ADSIDL=1),
    // S/H in Sample (SAMP = 1)
    AD1CON2 = 0; // Configure A/D voltage reference
    // and buffer fill modes.
    // Vr+ and Vr- from AVdd and AVss(PVCFG<1:0>=00, NVCFG=0),
    // Inputs are not scanned,
    // Interrupt after every sample
    AD1CON3 = 0; // Configure sample time = 1Tad,
    // A/D conversion clock as Tcy
    AD1CHS = 0; // Configure input channels,
    // S/H+ input is AN0,
    // S/H- input is Vr- (AVss).
    AD1CSSL = 0; // No inputs are scanned.
    IFS0bits.AD1IF = 0; // Clear A/D conversion interrupt.
    // Configure A/D interrupt priority bits (AD1IP<2:0>) here, if
    // required. Default priority level is 4.
    IEC0bits.AD1IE = 1; // Enable A/D conversion interrupt
    AD1CON1bits.ADON = 1; // Turn on A/D

     xAdcQueue = xQueueCreate(10, sizeof(float));
     
     // Tick rate == 1000Hz
    xAdcTimer = xTimerCreate("AdcTimer", 10 / portTICK_PERIOD_MS, pdTRUE, (void *) 0, vAdcTimerCallback);
    xTimerStart(xAdcTimer, 0);

}

// Interrupt when AD1IF is set
// Calculates the input current from the ADC voltage. 
void __attribute__ ((__interrupt__)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
    
    // The simulated_adc_result variable is what the ADC buffer register would
    // be if the ADC was actually sampling from a real input voltage.
    unsigned short int simulated_adc_result;

    // For the first 11 samples, every sample is incremented by .5v
    // sequentially. After that, the "sampled" voltage is a random voltage
    // between 0 and 5V.
    if (input_counter > 10){
        simulated_adc_result = rand() % 4095;
    }
    else {
         simulated_adc_result = (inputs[input_counter++] * 4095 / 5) + 0.5;
    }
 
    float adc_voltage = (float) simulated_adc_result * 5.0 / 4095.0;
    
    float amps = adc_voltage / 250.0;
    float adc_milliamps = amps * 1000.0;
    
    xQueueSend(xAdcQueue, (void *) &adc_milliamps, ( TickType_t ) 0);
}
