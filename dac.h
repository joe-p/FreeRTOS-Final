// Converts decimal voltage to digital value for output
void output_to_dac(float output_voltage);

// Performs necessary math to convert the input to the proper output voltage
float output_logic(float input_milliamps);

// Initializes TRISA register to all zeros to designate PORTA is an output and 
// creates the necessary tasks and queue
void dac_init();

// Receives values from xAdcQueue, sends them to output_logic(), the sends the
// result to  xOutputQueue
void vLogicTask( void *pvParameters );

// Receives output values from xOutputQueue and sends them to output_to_dac() 
void vOutputTask( void *pvParameters );