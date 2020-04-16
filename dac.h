#include "xc.h"
#include <float.h>


void output_to_dac(float output_voltage);
float output_logic(float input_milliamps);
void dac_init();
void vLogicTask( void *pvParameters );
void vOutputTask( void *pvParameters );