#include "dac.h"
#include "xc.h"
#include <float.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t  xAdcQueue;
QueueHandle_t xOutputQueue;

// Initializes TRISA register to all zeros to designate PORTA is an output and 
// creates the necessary tasks and queue. 
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

// Converts decimal voltage to digital value for output
void output_to_dac(float output_voltage){  
    unsigned short int output_bits = (output_voltage * 4095 / 5) + 0.5;
    
    PORTA = output_bits;
}

// Performs necessary math to convert the input to the proper output voltage
float output_logic(float input_milliamps){
    float output_milliamps;
    
    output_milliamps =  -.75 * input_milliamps + 21;

    if ( ( input_milliamps < (4 - FLT_EPSILON)) || ( input_milliamps > (20 + FLT_EPSILON)) ) {
        output_milliamps = 2;
    }
    
    float output_voltage = 250 * output_milliamps / 1000;
    
    return output_voltage;
    
}

// Receives values from xAdcQueue, sends them to output_logic(), the sends the
// result to  xOutputQueue
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

// Receives output values from xOutputQueue and sends them to output_to_dac() 
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

