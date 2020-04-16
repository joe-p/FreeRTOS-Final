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
   
   xTaskCreate(	vLogicTask,		/* Pointer to the function that implements the task. */
      "Receiver",	/* Text name for the task.  This is to facilitate debugging only. */
      1000,		/* Stack depth - most small microcontrollers will use much less stack than this. */
      NULL,		/* We are not using the task parameter. */
      2,			/* This task will run at priority 2. */
      NULL );		/* We are not using the task handle. */
   
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

void vLogicTask( void *pvParameters )
{
  float rxVal;
  
  /* As per most tasks, this task is implemented in an infinite loop. */
  for( ;; )
  {
    if (xAdcQueue != 0)
    {
      // Receive a message on the created queue.  Block for 100 ticks if a
      // message is not immediately available.
      if (xQueueReceive(xAdcQueue, &( rxVal ), (TickType_t)100))
      {
          output_logic(rxVal);
      }
    }
  }
}

