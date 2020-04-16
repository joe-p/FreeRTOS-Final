/*
 * File:   adc.c
 * Author: joe
 *
 * Created on April 7, 2020, 9:47 AM
 */
#include "adc.h"
#include <stdlib.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"   
#include "queue.h"

TimerHandle_t xAdcTimer;
QueueHandle_t  xAdcQueue;


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

     xAdcQueue = xQueueCreate(10, sizeof(char*));

}

void adc_sample(void){
 AD1CON1bits.SAMP = 1; // Start sampling the input
 __delay32(1500000); // delay some amount of time
 // before starting conversion.
 AD1CON1bits.SAMP = 0; // End A/D sampling and start conversion
}

int input_counter = 0;
float inputs[7] = { 0, 0.5, 1, 1.5, 2, 2.5, 3 };

float adc_milliamps;
float adc_voltage; 

void __attribute__ ((__interrupt__)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
        
    float input_voltage;
    
    if (input_counter > 7){
        input_voltage = inputs[ rand() % 7];
    }
    else {
        input_voltage = inputs[input_counter++];
    }
    
    int simulated_input = input_voltage * 13650.0/ 11.0;
    
    adc_voltage = simulated_input / (4095.0 / 3.3);
    
    float amps = adc_voltage / 250.0;
    adc_milliamps = amps * 1000.0;
    
    if (xQueueSend(xAdcQueue, (void *) &adc_milliamps, ( TickType_t ) 0) != pdTRUE ) {
        int breakvar = 1;
    }
}
