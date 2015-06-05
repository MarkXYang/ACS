/*
 * Ecosat2_CANBus.h
 *
 * Created: 10/09/2014
 *  Author: Cass
 *
 *	This is the code written by Cass for the Can Bus. Originally all in the main file, Reorganized by Brad.
 *	
 *	
 *	
 */ 

#ifndef _ECOSAT2_CANBUS_H_
#define _ECOSAT2_CANBUS_H_
//_____ I N C L U D E S ________________________________________________________

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

//Data type include files
#include "protocol.h"
#include "Ecosat2_messages.h"

//_____ D E F I N I T I O N S _____________________________________________________________________
#define MY_TAG		(U32)1
/*
#################################################
 MY_TAG is the System Identifier (Make sure this is set to the correct value for your system
CPU						0
ACDS					1
Communication Modem		2
Power					3
[open]					4
[open]					5
Payload					6
Broadcast				7

#################################################
*/

//_____ M A C R O S _______________________________________________________________________________
//Format the needed fields for
#define format_message_id(destination, type, command) ((((U32)destination << message_destination_id_offset) & message_destination_id_mask) | (((U32)type << message_command_offset) & message_command_mask) | (((U32)command << message_command_data_offset) & message_command_data_mask))


		

//_____ D E C L A R A T I O N S ___________________________________________________________________


/*
#################################################
		Initializations
#################################################
*/

void canBusInit(void);

	/*
		This function initializes the CAN peripheral for operation with our application layer protocol.
		The peripheral is so there are specific mailbox's and identifier filters for reception and transmission of extended identifier messages,
			with identifier filters setup correctly the device will ignore all messages with destination IDs not matching its own.
	*/

void reservation_init(void);

	/*
		Sets up memory space and timers for CAn bus timeout handling.
	*/

/*
#################################################
		Tasks
#################################################
*/


	
//CAN Bus Handlers-------------------------------
void canBusHandler(xQueueHandle incommingOBCMessage);

	/*
		This task deals with interpreting the received CAN messages, you will need to add your function calls and specific messages in here.
	*/
	
void canBusTransmit(void);

	/*
		This task deals with the queue of messages to be sent on the CAN bus.
	*/

void canBusTimeout(void);

	/*
		Task that handles CAN message timeouts.
	*/

	
//CAN Bus Physical Layer-------------------------

//RTOS Hooks-------------------------------------
void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName );

	/*
		This must send a message to the OBC as a backup in the
	*/	

/*
####################################################
		Other Functions
####################################################
*/
U8 can_transmit(can_format message);

	/*
	This Function is used to setup a mailbox for the transmission of a extended identifier can message
	*/
	
U8 can_transmit_remote(can_format message);

	/*
		This function is used exactly like the normal can message format but for remote message types (read documentation regarding the physical CAN protocol);
	*/

void ReservationTimerCallback( xTimerHandle pxTimer );

	/*
		This is the call back function for the FreeRTOS timers used to track CAN message timeouts,
		When a timer expires this function is called to determine which message it relates to then alerts the timer task through the can_timeout_queue queue
	*/
	
void reservation_init(void);

	/*
		Initialization function for setting up the 8 reservation stations used for transmitted messages, this allows us to have up to 8 messages waiting for acknowledgment at a time
	*/

void canBusReceive(void);

	/*
		
	*/
	
void easyCAN(Bool fromISR, U8 destination_id, U8 command_data, U8 dlc, U8* data);

	/*
		Created in order to easily send CAN messages using default parameters. Must set first parameter to true if calling function is an ISR.
		If you don't know what an ISR is, set false.
	*/



#endif