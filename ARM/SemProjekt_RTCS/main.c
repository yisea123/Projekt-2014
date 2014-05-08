/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: main.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 0902012  KHA   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "inc\lm3s6965.h"
#include "inc\emp_type.h"
#include "cpu\cpu.h"
#include "rtcs\rtcs.h"
#include "debug/debug.h"
#include "uartprintf.h"
#include "queue.h"
#include "spi.h"
#include "controller/controller_task.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
void init_alive_task(){
	//debug_pin(ON);
	// Start task
	_start(ALIVE_TASK, MILLI_SEC(500));
	UARTprintf("Starting up\r\n",0);

}

void alive_task(void)
{
	// Toggles status LED every 1 sec.
	GPIO_PORTF_DATA_R ^= 0x01;

	// Wait 1 sec.
	INT16U receivedChar;
	INT16U sendChar;
	//sendChar = 0b101000000110;
	//QueueSend(QueueSPITX, &sendChar);


	//output queue - if anything is available
	//if(QueueReceive(QueueSPIRX, &receivedChar)){
		//UARTprintf("received hex from SPI: %X \r\n", receivedChar);
	//}
	//debug_pin_red(OFF);
	//debug_pin(TOGGLE);
	//debug_pin_red(TOGGLE);
	_wait(MILLI_SEC(500));
}





int main(void)
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
	disable_global_int();
	clk_system_init();
	uartprintf_init();
	spi_init();
	gpio_init();

	/* Initialize queueHanders */
	QueueUARTTX 		= CreateQueueHandle();
	QueueUARTRX 		= CreateQueueHandle();
	QueueSPITX  		= CreateQueueHandle();
	QueueSPIRX  		= CreateQueueHandle();
	QueuePositionPAN 	= CreateQueueHandle();
	QueuePositionTILT 	= CreateQueueHandle();
	QueuePWMOutTilt		= CreateQueueHandle();

	enable_global_int();

	start_rtcs_scheduler();		// Call scheduler

	while(1);

	return( 0 );
}

/****************************** End Of Module *******************************/