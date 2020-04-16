/*
 * File:   dac.c
 * Author: joe
 *
 * Created on April 15, 2020, 10:39 PM
 */


/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"   
#include "queue.h"
#include "dac.h"

float output_voltage; 
extern float adc_voltage;
extern float adc_milliamps;

extern QueueHandle_t  xAdcQueue;

void dac_init(){
    TRISA = 0b00000000;
   
}

void output_to_dac(){  
    unsigned int output_bits;
    
    // Assuming 12-bit DAC ranging from 0 to 255
    output_bits = output_voltage * 13650 / 11;
    
    PORTA = output_bits;
    
}

void output_logic(float input_milliamps){
    float output_milliamps;
    
    output_milliamps =  -.75 * input_milliamps + 21;

    if ( ( input_milliamps < (4 - FLT_EPSILON)) || ( input_milliamps > (20 + FLT_EPSILON)) ) {
        output_milliamps = 2;
    }
    
    output_voltage = 250 * output_milliamps / 1000;
    
}

