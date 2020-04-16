/*
 * File:   dac.c
 * Author: joe
 *
 * Created on April 15, 2020, 10:39 PM
 */
#include "dac.h"
#include "xc.h"
#include <float.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t  xAdcQueue;
QueueHandle_t xOutputQueue;

void dac_init(){
    TRISA = 0b00000000;
   
   xOutputQueue = xQueueCreate(10, sizeof(float));

   xTaskCreate(	vLogicTask,		/* Pointer to the function that implements the task. */
      "Receiver",	/* Text name for the task.  This is to facilitate debugging only. */
      200,		/* Stack depth - most small microcontrollers will use much less stack than this. */
      NULL,		/* We are not using the task parameter. */
      3,			/* This task will run at priority 2. */
      NULL );		/* We are not using the task handle. */
   
      xTaskCreate(	vOutputTask,		/* Pointer to the function that implements the task. */
      "Receiver",	/* Text name for the task.  This is to facilitate debugging only. */
      200,		/* Stack depth - most small microcontrollers will use much less stack than this. */
      NULL,		/* We are not using the task parameter. */
      2,			/* This task will run at priority 2. */
      NULL );		/* We are not using the task handle. */
   
}

void output_to_dac(float output_voltage){  
    unsigned int output_bits;
    
    // Assuming 12-bit DAC ranging from 0 to 4095
    output_bits = output_voltage * 13650 / 11;
    
    PORTA = output_bits;
    
}

float output_logic(float input_milliamps){
    float output_milliamps;
    
    output_milliamps =  -.75 * input_milliamps + 21;

    if ( ( input_milliamps < (4 - FLT_EPSILON)) || ( input_milliamps > (20 + FLT_EPSILON)) ) {
        output_milliamps = 2;
    }
    
    float output_voltage = 250 * output_milliamps / 1000;
    
    return output_voltage;
    
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
          float output_value = output_logic(rxVal);
          
          xQueueSend(xOutputQueue, (void *) &output_value, ( TickType_t ) 0);

      }
    }
  }
}

void vOutputTask( void *pvParameters )
{
  float rxVal;
  
  /* As per most tasks, this task is implemented in an infinite loop. */
  for( ;; )
  {
    if (xOutputQueue != 0)
    {
      // Receive a message on the created queue.  Block for 100 ticks if a
      // message is not immediately available.
      if (xQueueReceive(xOutputQueue, &( rxVal ), (TickType_t)100))
      {
          output_to_dac(rxVal);
      }
    }
  }
}

