/*
 * Ecosat2_UART.h
 *
 * Created: 10/09/2014
 *  Author: Brad Maibach
 *
 *	This is the code written by Cass for the UART. Originally all in the main file, Reorganized by Brad.
 *	
 *
 *	
 */ 

#ifndef _ECOSAT2_UART_H_
#define _ECOSAT2_UART_H_
//_____ I N C L U D E S ___________________________________________________________________________
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "croutine.h"

//AT90CAN include files
#include "config.h"
#include "can_lib.h"
#include "compiler.h"
#include "uart.h"
#include "spi_lib.h"

//Peripheral include files
#include "ADT7320_TemperatureSensor.h"

//Data type includes
#include "protocol.h"
#include "Ecosat2_messages.h"

//_____ D E F I N I T I O N S _____________________________________________________________________
#define URx    0x02
#define UTx    0x01

//_____ M A C R O S _______________________________________________________________________________

//_____ D E C L A R A T I O N S ___________________________________________________________________
/*
#################################################
		Global Variables
#################################################
*/
//Free RTOS Queues-------------------------------
xQueueHandle uart_receive_queue;
xQueueHandle uart_transmit_queue;
xQueueHandle uart_transmit_queue_physical;

/*
#################################################
		Initializations
#################################################
*/
void UART_INIT_TEMP(void);

	/*
	Initialization function for UART1 (if UART0 is used in your design change all the registers below)
	Please Read The UART section of the AT90CAN data sheet at svn://ecosat3.engr.uvic.ca/svnrepos/Bus/Datasheets/AT90CAN/AT90CAN.pdf before performing modifications to this code
	*/

/*
#################################################
		Tasks
#################################################
*/
void uart_handler();

	/*
		Uart reception handler task, This task was setup for the umbilical program so if you require the uart just empty everything in the for(;;) loop and impliment your code
	*/
	
void uart_transmit_handler();

	/*
		Uart transmit task, This task was setup for the umbilical program so if you require the uart just empty everything in the for(;;) loop and impliment your code
		take note that when sending longer packets the uart_transmit_queue_physical is filled starting with the 2nd byte to initiate transmission load the first byte into UDR1
		once UDR1 is loaded it will transmit and the TX interrupt will empty the recent of the queue
	*/
/*
#################################################
		Other Functions
#################################################
*/

#endif