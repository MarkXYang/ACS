/*
 * Ecosat2_CANBus.c
 *
 * Created: 10/09/2014
 *  Author: Brad Maibach 
 *
 *	This is the code written by Cass for the Can Bus. Originally all in the main file, Reorganized by Brad.
 *	
 *
 *	
 */ 

//_____ I N C L U D E S ___________________________________________________________________________
#include "../Include/Ecosat2_CANBus.h"

//_____ D E F I N I T I O N S _____________________________________________________________________

//_____ M A C R O S _______________________________________________________________________________
/*
#################################################
Global Variables
#################################################
*/
//Bus Configuration Parameters-------------------
U8	code_version = 1;

//U8 max_number_of_resends = reservation_default_max_number_of_trys;
U8 max_number_of_resends = 2;
U16 can_bus_timeout = 200;
U16 subsystem_serial_timeout = 1000;

//Can Bus reception------------------------------
st_cmd_t receive_mob_local; //can interrupt receive object from local ID mask
st_cmd_t receive_mob_broadcast; //can interrupt receive object from broadcast ID mask
//st_cmd_t isp_acknowledgement_mob;
//U8 isp_acknowledgement_mob_data[8] = 0,0,0,0,0,0,0,0}; //Memory space for can message reception
U8 receive_mob_data_local[8]       = {0,0,0,0,0,0,0,0}; //Memory space for can message reception
U8 receive_mob_data_broadcast[8]   = {0,0,0,0,0,0,0,0}; //Memory space for can message reception
can_format acknowledgement_command;

//Can Bus transmit-------------------------------
st_cmd_t transmit_mob; //mob used for actual transmission
st_cmd_t acknowledgement_mob; //mob used for ack transmission
//st_cmd_t isp_transmit_mob;
volatile reservation_station transmit_reservation_stations[8];
volatile U8 transmit_sequence_id[8] = {0,0,0,0,0,0,0,0};

//Free RTOS Queues-------------------------------
xQueueHandle can_receive_queue;
xQueueHandle can_transmit_queue;
xQueueHandle can_transmit_queue_physical;
xQueueHandle can_transmit_status_queue;
xQueueHandle can_timeout_queue;
xQueueHandle obcMessageQueue;

//Mutexs and Semaphores--------------------------
xSemaphoreHandle reservation_station_mutex;
xSemaphoreHandle can_transmit_mutex;
xSemaphoreHandle reception_binary_semaphore;

//_____ I M P L E M E N T A T I O N S _____________________________________________________________
/*
#################################################
		Initializations
#################################################
*/
void canBusInit(void)
{
	//Create Queues:
	//queue of can messages to be sent
	can_transmit_queue = xQueueCreate(5,sizeof(can_format));
	
	//queue of can messages received
	can_receive_queue = xQueueCreate(5, sizeof(can_format));
	
	//???
	can_timeout_queue = xQueueCreate(8,sizeof(U8));


	//Create Mutexs and Semaphores
	/*From Wikipedia:
	A useful way to think of a semaphore is as a record of how many units of a particular resource are available, coupled with operations to safely (i.e., without race conditions)
	adjust that record as units are required or become free, and, if necessary, wait until a unit of the resource becomes available. */
	
	/*mutual exclusion refers to the requirement of ensuring that no two concurrent processes[a] are in their critical section at the same time.*/
	
	//Because CAN messages cannot be sent two at a time, a mutex is needed to ensure that 2 tasks don't attempt to send messages at once.
	reservation_station_mutex = xSemaphoreCreateMutex();
	//?
	can_transmit_mutex = xSemaphoreCreateMutex();
	//?
	vSemaphoreCreateBinary(reception_binary_semaphore);
	
	//Register Queues and Mutexes for debugging
	//This allows these queues, mutexs, and semaphors to be visible in the FreeRTOS viewer extension (CTRL+ALF+F)
	vQueueAddToRegistry(can_receive_queue, (signed portCHAR *)"Can Bus Receive Queue"); //for debug
	vQueueAddToRegistry(can_transmit_queue, (signed portCHAR *)"Can Bus Transmit Queue");  //for debug
	vQueueAddToRegistry(reservation_station_mutex, (signed portCHAR *)"Station Mutex");
	vQueueAddToRegistry(reception_binary_semaphore, (signed portCHAR *)"Receive interrupt");
	
	
	//clear initial semaphore token
	//?
	xSemaphoreTake(reception_binary_semaphore, 0); 
	
	
	//! @param  Mode (for "can_fixed_baudrate" param not used)
	//!         ==0: start CAN bit timing evaluation from faster baudrate
	//!         ==1: start CAN bit timing evaluation with CANBTx registers

	can_init(0);
	
	//disable interrupts globally
	cli();
	
	//CANGIE is the CAN General Interrupt Enable register
	//ENRX enables the receive interrupt, ENIT enables all interrupts except for the CAN TImer Overrun interrupt
	CANGIE=((1<<ENRX)|(1<<ENIT)); //enable just RX and general for now
	
	
	//TODO: setup can error interrupt as well to detect nack situations
	//?
	sei();
	
	
	//Set broadcast ID reception object
	//Messages sent to the "broadcast" address will be received by all subsystems.
	//messages, once being received, are saved in the globally declared st_cmd_t (a.k.a. message object, or MOB) variables above.
	receive_mob_broadcast.id.ext = message_broadcast_ID << message_id_mask_offset;
	receive_mob_broadcast.ctrl.ide = 1;
	receive_mob_broadcast.ctrl.rtr = 0;
	receive_mob_broadcast.dlc = 8;
	receive_mob_broadcast.pt_data = &receive_mob_data_broadcast[0];
	receive_mob_broadcast.cmd = CMD_RX_MASKED;
	
	while(can_cmd(&receive_mob_broadcast) != CAN_CMD_ACCEPTED);
	can_enable_mob_interrupt(&receive_mob_broadcast);
	
	//Set local ID reception object
	//The local mob is used by the messages received by this subsystem
	//?
	receive_mob_local.id.ext = MY_TAG << message_id_mask_offset;
	receive_mob_local.ctrl.ide = 1;
	receive_mob_local.ctrl.rtr = 0;
	receive_mob_local.dlc = 8;
	receive_mob_local.pt_data = &receive_mob_data_local[0];
	receive_mob_local.cmd = CMD_RX_MASKED;
	
	while(can_cmd(&receive_mob_local) != CAN_CMD_ACCEPTED);
	can_enable_mob_interrupt(&receive_mob_local);
	
	/*isp_acknowledgement_mob.ctrl.ide = 0;
	isp_acknowledgement_mob.ctrl.rtr = 0;
	isp_acknowledgement_mob.dlc = 8;
	isp_acknowledgement_mob.pt_data = &isp_acknowledgement_mob_data[0];
	isp_acknowledgement_mob.cmd = CMD_RX_MASKED;
	while(can_cmd(&isp_acknowledgement_mob) != CAN_CMD_ACCEPTED);
	can_enable_mob_interrupt(&isp_acknowledgement_mob);*/
	
 	transmit_mob.ctrl.ide = 1;
 	transmit_mob.ctrl.rtr = 0;
 	transmit_mob.cmd = CMD_TX_DATA;
	 
	acknowledgement_mob.ctrl.ide = 1;
	acknowledgement_mob.ctrl.rtr = 0;
	acknowledgement_mob.cmd = CMD_TX_DATA;
	
	/*isp_transmit_mob.ctrl.ide = 0;
	isp_transmit_mob.ctrl.rtr = 0;
	isp_transmit_mob.cmd = CMD_TX_DATA;*/
	
	acknowledgement_command.dlc = 0;
	
	//This should enable the CAN transceiver
	PORTG |= 1;
}

void reservation_init(void)
{
	for(int i=0;i<8;i++){
		transmit_reservation_stations[i].station_timer = xTimerCreate((signed portCHAR *)"Station timer", can_bus_timeout, pdFALSE, (void *)i, ReservationTimerCallback);
		transmit_reservation_stations[i].station_timer_state = reservation_free;
		transmit_reservation_stations[i].station_ack_state = StateFree;
	}
}

/*
#################################################
		Tasks
#################################################
*/
//Watchdog---------------------------------------


//CAN Bus Handlers-------------------------------
void canBusHandler(xQueueHandle obcMessageQueue)
{	
	//Local memory allocation
	can_format receive_command;
	U8 volatile receive_source;
	U8 volatile receive_command_field;
	U32 volatile receive_command_data;
	U8 volatile receive_sequence;


	for(;;){
		if(xQueueReceive(can_receive_queue, &(receive_command), portMAX_DELAY)){
			
			receive_source = (receive_command.id & message_source_id_mask) >> message_source_id_offset;
			receive_command_field = (receive_command.id & message_command_mask) >> message_command_offset;
			receive_command_data = (receive_command.id & message_command_data_mask) >> message_command_data_offset;
			receive_sequence = (receive_command.id & message_message_num_mask) >> message_sequence_num_offset;	


			//TODO: check that this is not a duplicate message
			
			switch(receive_command_field){
				case message_cmd_send_data_packet:
					xQueueSend(obcMessageQueue, &receive_command, portMAX_DELAY);
				break;
				case message_cmd_send_data_packet_extended:
					break;
				case message_cmd_start_data_transfer:
					break;
				case message_cmd_send_data_transfer:
					if(receive_sequence != transmit_sequence_id[receive_source]){
						//transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 2) % 8;
					}else{
					//TODO Send GEN_BAD_SEQUENCE_ID
					}
					break;
				case message_cmd_request_data_transfer:

					break;
				case message_cmd_sequence_reset:
					//transmit_sequence_id[receive_source] = (U8)2;
					break;
				case message_cmd_non_responsive_alert:

					break;
				case message_cmd_bus_config:
					//if(receive_sequence != transmit_sequence_id[receive_source]){
						//transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 2) % 8;
						if(receive_command_data & (U32)0x1){
							can_bus_timeout = ((U16)receive_command.packet_data[2] << 8) | (receive_command.packet_data[3]);
						}
						if(receive_command_data & (U32)0x2){
							subsystem_serial_timeout = ((U16)receive_command.packet_data[0] << 8) | (receive_command.packet_data[1]);
						}
						if(receive_command_data & (U32)0x4){
							max_number_of_resends = (receive_command.packet_data[4]) & 0xFF;
						}
					//}
					break;
				case message_cmd_echo:
					//if(receive_sequence != transmit_sequence_id[receive_source]){
						if(receive_command_data & ( 1 << message_cmd_echo_request_offset)){
							//this is a request, just reuse the receive command memory space to format the response message
							receive_command.id &= ~(message_command_data_mask & message_destination_id_mask);
							receive_command.id |= ((U32)code_version << message_command_data_offset);
							receive_command.id |= ((U32)receive_source << message_destination_id_mask);
							xQueueSend(can_transmit_queue, (void *)&receive_command, 100);
						}
					//}
					break;
				case message_cmd_reset:
					if(receive_source == message_CPU_ID){
						wdt_enable(WDTO_30MS);
						while(1) {};
					}
					break;
				default:
					//TODO: in the future, deal with the application level isp commands
					//transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 2) % 8;
					break;

			}
			taskYIELD();
		}
	}	
}

void canBusTransmit()
{
	can_format volatile cmd_from_queue;
	U8 volatile destination;
	
	can_transmit_queue = xQueueCreate(5,sizeof(can_format)); //queue of can messages to be sent

	for(;;){
		if(xQueueReceive(can_transmit_queue, (void *) &(cmd_from_queue), portMAX_DELAY) == pdTRUE)
		{
			destination = (cmd_from_queue.id & message_destination_id_mask) >> message_destination_id_offset;
			if(destination != message_broadcast_ID){
				
				xSemaphoreTake( reservation_station_mutex, portMAX_DELAY);
				switch(transmit_reservation_stations[destination].station_ack_state){
					case StateFree:
					cmd_from_queue.id &= ~(message_message_num_mask | message_source_id_mask);
					cmd_from_queue.id |= (transmit_sequence_id[destination]<< message_sequence_num_offset) & message_message_num_mask;
					cmd_from_queue.id |= ((U32)MY_TAG << message_source_id_offset) & message_source_id_mask;
					transmit_reservation_stations[destination].station_message_1 = cmd_from_queue;
					transmit_reservation_stations[destination].station_timer_state = reservation_waiting;
					can_transmit(cmd_from_queue);
					xTimerStart(transmit_reservation_stations[destination].station_timer,0);
					transmit_reservation_stations[destination].station_ack_state = StateSent;
					transmit_sequence_id[destination] = (transmit_sequence_id[destination] + 1) % 8;
					break;
					case StateSent:
					xQueueSendToBack(can_transmit_queue, (void *) &(cmd_from_queue), portMAX_DELAY);
					break;
				}
				xSemaphoreGive(reservation_station_mutex);
				} else {
				can_transmit(cmd_from_queue);
			}
		}
	}
}

void canBusTimeout()
{
	can_timeout_queue = xQueueCreate(8,sizeof(U8));
	
	can_format alert_message;
	alert_message.dlc = 0;
	alert_message.id = (message_CPU_ID << message_destination_id_offset) | ((U32)message_cmd_non_responsive_alert << message_command_offset) | (MY_TAG << message_source_id_offset);
	U8 i = 0;

	for(;;){

		if(xQueueReceive(can_timeout_queue, (void *) &(i), portMAX_DELAY))
		{
			if(transmit_reservation_stations[i].station_timer_state != reservation_free){
				if(transmit_reservation_stations[i].station_timer_state > max_number_of_resends){ //> instead of == fixes the off by one issue
					//If we have reached the max number of retries give up and reset station number
					//TODO check if it was a message to the CPU that timed out
					if( (i > 0) && (i < 7) ){
						alert_message.id &= ~(message_command_data_mask | message_message_num_mask);
						alert_message.id |= transmit_sequence_id[message_CPU_ID] << message_sequence_num_offset;
						alert_message.id |= i << message_command_data_offset;
						xQueueSend(can_transmit_queue, (void *) &alert_message, 100);
						transmit_reservation_stations[i].station_timer_state = reservation_free;
						transmit_reservation_stations[i].station_ack_state = StateFree;
						xTimerStop(transmit_reservation_stations[i].station_timer,0);
						} else {
						transmit_reservation_stations[i].station_timer_state = reservation_free;
						transmit_reservation_stations[i].station_ack_state = StateFree;
						xTimerStop(transmit_reservation_stations[i].station_timer,0);
						//Broadcast or CPU message timedout
						//TODO: reset here
					}
					
					} else if(transmit_reservation_stations[i].station_timer_state != reservation_free){
						//not at the max, re-transmit
						if(xTimerIsTimerActive(transmit_reservation_stations[i].station_timer) == pdFALSE){
							switch(transmit_reservation_stations[i].station_ack_state){
								case StateFree:
								//TODO this state should be considered an error
								break;
								case StateSent:
								can_transmit(transmit_reservation_stations[i].station_message_1);
								//xQueueSend(can_transmit_queue_physical, (void *) &transmit_reservation_stations[i].station_message_1, portMAX_DELAY);
								xTimerStart(transmit_reservation_stations[i].station_timer,0);
								break;
							}
						}
				}
			}
		}
	}
}

//CAN Bus Physical Layer-------------------------
void canBusReceive()
{
	can_receive_queue = xQueueCreate(5, sizeof(can_format)); //queue of can messages received
	
	U8 savePage = CANPAGE;
	can_format receive_command;
	U8 receive_sequence;
	U8 receive_source;
	volatile U8 flag = 0;
	receive_command.ide = 1;
	
	//transmit_sequence_id[destination] = (transmit_sequence_id[destination] + 1) % 8;
	for(;;){
		if(xSemaphoreTake(reception_binary_semaphore, portMAX_DELAY) == pdTRUE)
		{
			savePage = CANPAGE;
			if(can_get_status(&receive_mob_local) == CAN_STATUS_COMPLETED){
				//TODO: Deal with error status correctly
				//TODO check
				CANSTMOB&=(~(1<<RXOK));//RXOK must be cleared.
				//receive_command.ide = 1;
				receive_command.id = receive_mob_local.id.ext;
				
				//acknowledgement_command.id |= ((receive_sequence + 1)<< message_sequence_num_offset) & message_message_num_mask;
				for(int i = 0; i<receive_mob_local.dlc; i++){
					receive_command.packet_data[i] = receive_mob_data_local[i];//pull out message data, to send to can Bus handler
				}
				receive_command.dlc = receive_mob_local.dlc;
				
				while(can_cmd(&receive_mob_local) != CAN_CMD_ACCEPTED);
				flag = 1;
				} else if (can_get_status(&receive_mob_broadcast) == CAN_STATUS_COMPLETED){
				//status = can_get_status(&receive_mob_broadcast);
				//TODO: Deal with error status correctly
				CANSTMOB&=(~(1<<RXOK));//RXOK must be cleared.
				//receive_command.ide = 1;
				receive_command.id = receive_mob_broadcast.id.ext;
				
				for(int i = 0; i<receive_mob_broadcast.dlc; i++){
					receive_command.packet_data[i] = receive_mob_data_broadcast[i];
				}
				receive_command.dlc = receive_mob_broadcast.dlc;
				while(can_cmd(&receive_mob_broadcast) != CAN_CMD_ACCEPTED);
				flag = 0;
				xQueueSend(can_receive_queue, (void *) &receive_command, portMAX_DELAY);
				} else {
				//default, dont know what happened so for now dont deal with it
				flag = 0;
			}
			
			can_enable_mob_interrupt(&receive_mob_local);
			can_enable_mob_interrupt(&receive_mob_broadcast);
			
			CANGCON &= ~(1 << OVRQ);
			
			if(flag == 1){ //This was a message directed to this system not to the broadcast address, therefore acknowledgements and nacks need to be delt with
				receive_source = (receive_command.id & message_source_id_mask) >> message_source_id_offset;
				receive_sequence = (receive_command.id & message_message_num_mask) >> message_sequence_num_offset;
				//xSemaphoreTake( reservation_station_mutex, portMAX_DELAY);
				if(receive_sequence != transmit_sequence_id[receive_source]){
					//sequence numbers are off, check if this is a message or an nack
					if(receive_command.id & 0x1){
						//message is an acknowledgment, This should never happen unless this system was reset between sending a receiving the message
						}else if(receive_command.id & 0x2){
						//message is an negative acknowledgment, need to correct the sequence number and fix the sequence number in the reservation station
						xSemaphoreTake( reservation_station_mutex, portMAX_DELAY);
						transmit_sequence_id[receive_source] = receive_sequence;
						switch(transmit_reservation_stations[receive_source].station_ack_state){
							case StateFree:
							//TODO this state should be considered an error
							break;
							case StateSent:
							transmit_reservation_stations[receive_source].station_message_1.id &= ~message_message_num_mask;
							transmit_reservation_stations[receive_source].station_message_1.id |= (receive_sequence << message_sequence_num_offset);
							transmit_reservation_stations[receive_source].station_timer_state = reservation_waiting;
							xTimerStop(transmit_reservation_stations[receive_source].station_timer,0);
							xQueueSend(can_timeout_queue, &receive_source, portMAX_DELAY);
							break;
						}
						transmit_sequence_id[receive_source] = receive_sequence + 1;
						xSemaphoreGive(reservation_station_mutex);
						}else {
						//message is a normal message, Need to send a nack but not change sequence numbers at all
						acknowledgement_command.id = receive_command.id;
						acknowledgement_command.id &= ~(message_source_id_mask | message_destination_id_mask | message_message_num_mask);
						acknowledgement_command.id |= (U32)((MY_TAG << message_source_id_offset) & message_source_id_mask);
						acknowledgement_command.id |= (U32)(((receive_command.id & message_source_id_mask) << message_destination_source_difference) & message_destination_id_mask);
						acknowledgement_command.id |= (U32)((transmit_sequence_id[receive_source] << message_sequence_num_offset) & message_message_num_mask);
						acknowledgement_command.id |= (U32)2;
						acknowledgement_command.dlc = 0;
						can_transmit_remote(acknowledgement_command);
						//xQueueSend(can_receive_queue, (void *) &receive_command, portMAX_DELAY);
					}
					} else {
					//sequence numbers are correct
					if(receive_command.id & 0x1){
						//message is an acknowledgement, clear reservation station
						xSemaphoreTake( reservation_station_mutex, portMAX_DELAY);
						transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 1) % 8;
						switch(transmit_reservation_stations[receive_source].station_ack_state){
							case StateFree:
							//TODO this state should be considered an error
							break;
							case StateSent:
							transmit_reservation_stations[receive_source].station_ack_state = StateFree;
							transmit_reservation_stations[receive_source].station_timer_state = reservation_free;
							xTimerStop(transmit_reservation_stations[receive_source].station_timer,0);
							break;
						}
						xSemaphoreGive(reservation_station_mutex);
						}else if(receive_command.id & 0x2){
						//message is an negative acknowledgement
						xSemaphoreTake( reservation_station_mutex, portMAX_DELAY);
						transmit_sequence_id[receive_source] = receive_sequence;
						switch(transmit_reservation_stations[receive_source].station_ack_state){
							case StateFree:
							//TODO this state should be considered an error
							break;
							case StateSent:
							transmit_reservation_stations[receive_source].station_message_1.id &= ~message_message_num_mask;
							transmit_reservation_stations[receive_source].station_message_1.id |= (receive_sequence << message_sequence_num_offset);
							break;
						}
						xSemaphoreGive(reservation_station_mutex);
						} else {
						//message is a normal message
						transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 1) % 8;
						acknowledgement_command.id = receive_command.id;
						acknowledgement_command.id &= ~(message_source_id_mask | message_destination_id_mask | message_message_num_mask);
						acknowledgement_command.id |= (U32)((MY_TAG << message_source_id_offset) & message_source_id_mask);
						acknowledgement_command.id |= (U32)(((receive_command.id & message_source_id_mask) << message_destination_source_difference) & message_destination_id_mask);
						acknowledgement_command.id |= (U32)((transmit_sequence_id[receive_source] << message_sequence_num_offset) & message_message_num_mask);
						acknowledgement_command.id |= (U32)1;
						can_transmit_remote(acknowledgement_command);
						transmit_sequence_id[receive_source] = (transmit_sequence_id[receive_source] + 1) % 8;
						xQueueSend(can_receive_queue, (void *) &receive_command, portMAX_DELAY);
					}
				}//if recieve sequence id != or =
			}//if flag = 1
			CANPAGE = savePage;
		} //if (semaphore)
	} //for (;;)
}

/*
#################################################
		Other Functions
#################################################
*/
U8 can_transmit(can_format message)
{
	U8 volatile Result;
	
	//Setup message object structure used by physical layer can library
	transmit_mob.id.ext = message.id;
	transmit_mob.dlc = message.dlc;
	transmit_mob.pt_data = &(message.packet_data[0]);

	
	//Enter critical section, This prevents the scheduler from performing a context switch
	taskENTER_CRITICAL();
	
	//Wait for the message structure to be accepted by the peripheral
	while((can_cmd(&transmit_mob) != CAN_CMD_ACCEPTED));
	
	//wait for the message to be transmitted (I cant think of a good way to move this from a polled event to a interrupt driven one)
	//For now all this really does is wait for it to be complete
	while((Result = can_get_status(&transmit_mob)) != CAN_STATUS_COMPLETED){
		if(Result == CAN_STATUS_ERROR){
			asm("nop");
			//TODO: deal with error correctly
			//can_cmd(&transmit_mob);
			//Hey Cass: ^^^ Has this been done?
		}
	}

	//Check if the message experienced a collision (ie overload frame was placed on the bus)
	if(CANGSTA & (1<<OVRG)){
		taskEXIT_CRITICAL();
		//Sending to fast back off a bit
 		for(U8 i = 0; i < 255; i++){
 			asm("nop");
 		}
		can_cmd(&transmit_mob);
		while(Result != CAN_STATUS_COMPLETED);
	}
	taskEXIT_CRITICAL();
	//xSemaphoreGive(can_transmit_mutex);
	return Result;	
}

U8 can_transmit_remote(can_format message)
{
	U8 volatile Result;
	//st_cmd_t transmit_cmd;
	//xSemaphoreTake(can_transmit_mutex, portMAX_DELAY);
	acknowledgement_mob.id.ext = message.id;
	//transmit_cmd.ctrl.ide = 1;
	//transmit_cmd.ctrl.rtr = 1;
	acknowledgement_mob.dlc = message.dlc;
	acknowledgement_mob.pt_data = &(message.packet_data[0]);
	acknowledgement_mob.cmd = CMD_TX_REMOTE;
	
	taskENTER_CRITICAL();
	while((can_cmd(&acknowledgement_mob) != CAN_CMD_ACCEPTED)); //should block here on complete or error
	Result = can_get_status(&acknowledgement_mob);
	while(Result != CAN_STATUS_COMPLETED){
		if(Result == CAN_STATUS_ERROR){
			//can_cmd(&acknowledgement_mob);
		}
		Result = can_get_status(&acknowledgement_mob);
	}

	if(CANGSTA & (1<<OVRG)){
		taskEXIT_CRITICAL();
		//Sending to fast back off a bit
		//vTaskDelay(1); // 1 ms delay
		for(U8 i = 0; i < 255; i++){
			asm("nop");
		}
		can_cmd(&acknowledgement_mob);
		while(Result != CAN_STATUS_COMPLETED);
	}
	taskEXIT_CRITICAL();
	//xSemaphoreGive(can_transmit_mutex);
	return Result;
}

void ReservationTimerCallback( xTimerHandle pxTimer )
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	U8 timerID;
	timerID = (int) pvTimerGetTimerID(pxTimer);
	
	transmit_reservation_stations[timerID].station_timer_state++;
	
	//xSemaphoreGiveFromISR(transmit_timeout_semaphore, &xHigherPriorityTaskWoken);
	xQueueSendFromISR(can_timeout_queue, &timerID, &xHigherPriorityTaskWoken);
	if(xHigherPriorityTaskWoken){
		taskYIELD();
	}
}



void easyCAN (Bool fromISR, U8 destination_id, U8 command_data, U8 dlc, U8* data)
{
	
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	
	can_format message;
	message.id = format_message_id(destination_id, 0, command_data);
	message.ide = 1;
	message.dlc = dlc;
	
	for(int i = 0; i < dlc; i++){
		message.packet_data[i] = data[i];
	}
	
	switch(fromISR){
		case true:
			xQueueSendFromISR(can_transmit_queue, &message, &xHigherPriorityTaskWoken);
			if(xHigherPriorityTaskWoken){
				taskYIELD();
			}
		break;
		
		case false:
			xQueueSend(can_transmit_queue, &message, (portTickType)portMAX_DELAY);
		break;
	}
}
/*
#################################################
		Interrupt Service Routines
#################################################
*/
ISR(CANIT_vect)
{
	signed portBASE_TYPE xHigherPriorityTaskWoken;
	CANIE1 = 0;
	CANIE2 = 0;

	CANGCON |= (1 << OVRQ);
	xSemaphoreGiveFromISR(reception_binary_semaphore, &xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken){
		taskYIELD();
	}
}
