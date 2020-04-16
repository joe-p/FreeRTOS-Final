/*
   FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
   All rights reserved

   VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

   This file is part of the FreeRTOS distribution.

   FreeRTOS is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License (version 2) as published by the
   Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.


   1 tab == 4 spaces!
   */

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"   
#include "queue.h"

// Project includes
#include "adc.h"
#include "dac.h"
#include "pic_config.h"

extern TimerHandle_t xAdcTimer;
extern QueueHandle_t  xAdcQueue;

/*-----------------------------------------------------------*/

int main( void )
{ 
    
  adc_init();
  dac_init();
 
  /* Start the scheduler to start the tasks executing. */
  vTaskStartScheduler();	

  /* The following line should never be reached because vTaskStartScheduler() 
     will only return if there was not enough FreeRTOS heap memory available to
     create the Idle and (if configured) Timer tasks.  Heap management, and
     techniques for trapping heap exhaustion, are described in the book text. */
  for( ;; );
  return 0;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}