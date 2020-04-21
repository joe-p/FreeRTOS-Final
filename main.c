/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

// Project includes
#include "pic_config.h"

/*-----------------------------------------------------------*/

int main( void )
{ 
  adc_init();
  dac_init();
 
  /* Start the scheduler to start the tasks executing. */
  vTaskStartScheduler();	

  /* The following line should never be reached because vTaskStartScheduler() 
     will only return if there was not enough FreeRTOS heap memory available to
     create the Idle and Timer tasks. */
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