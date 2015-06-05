
/*
####################################################
Main function for the ACS subsystem
####################################################
*/
//TODO: STATUS request response for temperature
//TODO: overflow hook
//TODO: rework axis control handling
//TODO: FreeRTOS integration of peripherals


//_____ Header file includes __________________________________________________

#include "ACS.h"

//_____ Misc Function Declarations ______________________________________________
void watchdogTimerReset();

//_____ Global Variables ______________________________________________
U32 temperature;
U8 hb_faults[6];
U8 hb_enabled[6];
U8 temp_status;

/**********************************Create command queues********************************************************/
xQueueHandle obcMessageQueue;

/**********************************Create output queues********************************************************/
xQueueHandle axisControlQueue;

/**********************************Create peripheral semaphores********************************************************/
xSemaphoreHandle hb_semaphore[6];
xSemaphoreHandle temp_semaphore;

//_____ Main Function __________________________________________________
int main(void)
{
	/**********************************Initializations********************************************************/
	acs_init();
	
	/**********************************Start Tasks and scheduler********************************************************/	
	xTaskCreate((void *)canBusReceive, (const signed portCHAR *)"CAN RX", 300, NULL, 5, NULL);
	xTaskCreate((void *)canBusTransmit, (const signed portCHAR *)"CAN TX", 300, NULL, 5, NULL);
	xTaskCreate((void *)canBusTimeout, (const signed portCHAR *)"CAN TO", 300, NULL, 5, NULL);
	xTaskCreate(acs_messageHandler, (const signed portCHAR *)"OBCMSG", 300, NULL, 4, NULL);
	vTaskStartScheduler();
	
	return 0;
}
//_____ Tasks __________________________________________________

void acs_messageHandler()
{
	/**********************************Start Sub-tasks********************************************************/
	//start task to fill command queue
	can_format obc_message_buffer;
	U8 volatile receive_command_id_buffer;
	xTaskCreate(canBusHandler, (const signed portCHAR *)"CAN HAND", 255, &obcMessageQueue, 4, NULL);

	//start tasks for axis control output
	xTaskCreate(acs_axisController, (const signed portCHAR *)"AXIS", 2, NULL, 3, NULL);
	
	for(;;){
		xQueueReceive(obcMessageQueue, &obc_message_buffer, portMAX_DELAY);
		
		receive_command_id_buffer = (obc_message_buffer.id & message_command_mask) >> message_command_offset;
		
		switch(receive_command_id_buffer){
				/**********************************Message Cases********************************************************/
                
                case ACS_STATUS_REQUEST_ID:
                //obc_message_buffer.packet_data contains null
					if(obc_message_buffer.packet_data[TEMP] == TRUE){
						acs_status_temp_response();
					}
					if(obc_message_buffer.packet_data[FAULT] == TRUE){
						acs_status_fault_response();
					}
                break;

                case ACS_AXIS_CTRL_REQUEST_ID:
					xQueueSend(axisControlQueue, &obc_message_buffer.packet_data, MAX_AXIS_COMMAND_DURATION);				
                break;
				
				case ACS_HBRIDGE_ENABLE_REQUEST_ID:
					acs_hb_enable_response(obc_message_buffer.packet_data);
				break;
				
				case ACS_DEGAUSS_REQUEST_ID:
					acs_degauss_response(obc_message_buffer.packet_data);
				break;
		}
	}
}

void acs_axisController()
{
	//Buffer for received command
	U8* packet_data_buffer = NULL;
	
	U8 x_control_buffer;
	U8 x_duration_buffer;
	U8 y_control_buffer;
	U8 y_duration_buffer;
	U8 z_control_buffer;
	U8 z_duration_buffer;
	
	//this function alternates using primary and secondary magnetorquers
	//on each axis. This prevents overcurrent conditions from occuring.
	//It is recommended that a single magnetorque not be run on full power for more than 250 ms!
	int X1orX2 = X1;
	int Y1orY2 = Y1;
	int Z1orZ2 = Y2;
	
	xTimerHandle xShutoff = NULL;
	xTimerHandle yShutoff = NULL;
	xTimerHandle zShutoff = NULL;
		
	for(;;){
		
		xQueueReceive(axisControlQueue, packet_data_buffer, portMAX_DELAY);
		x_control_buffer = packet_data_buffer[X_CONTROL];
		x_duration_buffer = packet_data_buffer[X_DURATION_MS];
		y_control_buffer = packet_data_buffer[Y_CONTROL];
		y_duration_buffer = packet_data_buffer[Y_DURATION_MS];
		z_control_buffer = packet_data_buffer[Z_CONTROL];
		z_duration_buffer = packet_data_buffer[Z_DURATION_MS];
		
		xSemaphoreTake(hb_semaphore[X1], portMAX_DELAY);
		xSemaphoreTake(hb_semaphore[X2], portMAX_DELAY);
		set_control_register(X1orX2, x_control_buffer);
		xShutoff = xTimerCreate
						((const signed char * const)"xShutoff",
						x_duration_buffer,
						FALSE,
						(void *)X1orX2,
						magnetorquer_off_callback);
		if(X1orX2 == X1 && hb_enabled[X2] == TRUE){
			X1orX2 = X2;
		}else if(X1orX2 == X2 && hb_enabled[X1] == TRUE){
			X1orX2 = X1 ;
		}
		xSemaphoreTake(hb_semaphore[Y1], portMAX_DELAY);
		xSemaphoreTake(hb_semaphore[Y2], portMAX_DELAY);
		set_control_register(Y1orY2, y_control_buffer);
		
		yShutoff = xTimerCreate
						( (const signed char * const)"yShutoff",
						y_duration_buffer,
						FALSE,
						(void *)Y1orY2,
						magnetorquer_off_callback);               
		if(Y1orY2 == Y1 && hb_enabled[Y2] == TRUE){
			X1orX2 = Y2;
			}else if(Y1orY2 == Y2 && hb_enabled[Y1] == TRUE){
			Y1orY2 = Y1 ;
		}
		set_control_register(Z1orZ2, z_control_buffer);		
		xSemaphoreTake(hb_semaphore[Z1], portMAX_DELAY);
		xSemaphoreTake(hb_semaphore[Z2], portMAX_DELAY);
		
		zShutoff = xTimerCreate
						( (const signed char * const)"zShutoff",
						z_duration_buffer,
						FALSE,
						(void *)Z1orZ2,
						magnetorquer_off_callback);
    	if(Z1orZ2 == Z1 && hb_enabled[Z2] == TRUE){
			Z1orZ2 = Z2;
		}else if(Z1orZ2 == Z2 && hb_enabled[Z1] == TRUE){
			Z1orZ2 = Z1 ;
		}
	}	
}


//_____ CAN Responses __________________________________________________

void acs_status_fault_response()
//ACS must respond by attaching the state of all 6 H-bridge fault registers
{
	
	for (int hb = 0; hb < 6; hb++){
		xSemaphoreTake(hb_semaphore[hb], portMAX_DELAY);
		hb_faults[hb] = get_fault(hb);
		xSemaphoreGive(hb_semaphore[hb]);
	}
	
	U8 attach_me[6];
	for(int i = 0; i < 6; i++){
		attach_me[i] = hb_faults[i];
	}
	
	easyCAN(FALSE, 0, ACS_STATUS_FAULT_RESPONSE_ID, 6, (unsigned char *)attach_me);
}

void acs_status_temp_response()
//ACS must respond via CAN attaching the 2 temperature bytes
{
	xSemaphoreTake(temp_semaphore, portMAX_DELAY);
	temperature = ACS_ADT7320_READ_TEMPERATURE();
	xSemaphoreGive(temp_semaphore);
	
	U8 temp_msg[3];
	temp_msg[TEMP_LOW] = (temperature & 0x00FF);
	temp_msg[TEMP_HIGH] = (temperature & (0xFF00) >> 8);
		
	easyCAN(FALSE, 0, ACS_STATUS_TEMP_RESPONSE_ID, 2, (unsigned char *)temp_msg);
}

void acs_degauss_response(U8* packet_data)
//Degaussing will take place prior to the experiment
{
	U8 degaussed[6];
	for(int hb = 0; hb < 6; hb++){
		if(packet_data[hb] == TRUE){
			xSemaphoreTake(hb_semaphore[hb], portMAX_DELAY);
			for(int i = 0; i <= DEGAUSSING_FINAL_INDEX; i++){
				set_control_register(hb, get_controlbyte(i));
			}
		}else{
			degaussed[hb] = FALSE;
		}
		xSemaphoreGive(hb_semaphore[hb]);
		degaussed[hb] = TRUE;
	}
	easyCAN(FALSE, 0, ACS_DEGAUSS_RESPONSE_ID, 6, (unsigned char *)degaussed);
}
void acs_hb_enable_response(U8* packet_data){
	for(int hb = 0; hb < 6; hb++){
		hb_enabled[hb] = packet_data[hb];	
	}
	easyCAN(FALSE, 0, ACS_HBRIDGE_ENABLE_RESPONSE_ID, 6, (unsigned char *)hb_enabled);
}
//_____ Initializations __________________________________________________

void acs_init(void)
{	
	/**********************************Power Saving********************************************************/
	//The brown out detector, JTAG and on-chip debugging fuses must be disabled in the fuses section of the device programming window in the final version!!
	
	//clock source select
	ASSR &= (0<<AS2);
	
	//select idle mode as the used sleep mode
	SMCR &= (0<<SM0) | (0<<SM1) | (0<<SM2);
	
	//enable the usage of the sleep mode
	SMCR |= (1<<SE);
	
	//disable ADC
	ADCSRA &= (0<<ADEN);
	
	//disable analog comparator
	ACSR |= (1<<ACD);
	
	/**********************************Temp Sensor********************************************************/
	//slave select data direction
	DDRB |= ACS_ADT7320_SS_PIN;
	
	//Direct signal data direction
	DDRF |= ACS_OVRTMP;
	
	//SPI initialization
	spi_init(ACS_SPI_CONFIG_BYTE);

	//Temp. sensor configuration
	ACS_ADT7320_WRITE_CONFIGURATION(ADT7320_CONFIG_BYTE);

		
	/**********************************H-Bridges********************************************************/
	//Set data direction registers, and values to high.	
	DDRD |= DRV8830_SCL_PIN | DRV8830_SDA_PIN;
	PORTD |= DRV8830_SCL_PIN | DRV8830_SDA_PIN;
	
	//Direct signal data direction
	DDRF |= ACS_nFAULT;
	
	i2c_init();
	
	//data direction registers of HB fault pins
	DDRE &= ~(X1_FAULT_PIN|X2_FAULT_PIN|Y1_FAULT_PIN|Y2_FAULT_PIN);
	DDRD &= ~(Z1orZ2_FAULT_PIN);
	
	//Set all H-Bridges enabled
	for(int hb = 0; hb < 6; hb++){
		hb_enabled[hb] = TRUE;
	}
	
	/**********************************Interrupts********************************************************/
	//Configure interrupts for h-bridge
	EICRA |= ACS_EICRA;
	EICRB |= ACS_EICRB;
	
	//Enables these interrupts
	EIMSK |= ACS_EIMSK; 
	
	/**********************************Initialize queues********************************************************/
	obcMessageQueue = xQueueCreate(255, sizeof(can_format));
	axisControlQueue = xQueueCreate(32, sizeof(U8*));
	
	vQueueAddToRegistry(obcMessageQueue, (signed portCHAR *)"Incomming OBC Messages");
	vQueueAddToRegistry(axisControlQueue, (signed portCHAR *)"outgoing X commands");
	
	/**********************************Initialize semaphores********************************************************/
	for(int i = 0; i < 6; i++){
		vSemaphoreCreateBinary(hb_semaphore[i]);
	}
	
	vSemaphoreCreateBinary(temp_semaphore);
}

//_____ H-Bridge Functions __________________________________________________

int get_hb_write_address(int hb){return (0xC0 + hb*2);}
int get_hb_read_address(int hb){return (0xC1 + hb*2);}

void set_control_register(int hb, unsigned char control_byte)
{
	i2c_start(0xC0 +2*hb+TWI_W);
	i2c_write(HB_CONTROL_SUBADDRESS);
	i2c_write(control_byte);
	i2c_stop();
}

unsigned char get_fault(int hb)
{
	unsigned char fault_byte;
	i2c_start_wait(0xC0 +2*hb + TWI_W);
	i2c_write(HB_FAULT_SUBADDRESS);
	i2c_rep_start(0xC0 + 2*hb + TWI_R);
	fault_byte = i2c_readNak();
	i2c_stop();
	return fault_byte;
}

void clear_fault(int hb)
{
	i2c_start(get_hb_write_address(hb));
	i2c_write(HB_FAULT_SUBADDRESS);
	i2c_write(1<<CLEAR_OFFSET);
	i2c_stop();
}

void magnetorquer_off_callback(xTimerHandle pxTimer)
{
	U8 active_hb = (int)pvTimerGetTimerID(pxTimer);
	U8 partner_hb = (active_hb + 3) % 6;
	i2c_start(get_hb_write_address(active_hb));
	i2c_write(HB_CONTROL_SUBADDRESS);
	i2c_write(COAST<<IN10_OFFSET);
	i2c_stop();
	xSemaphoreGive(active_hb);
	xSemaphoreGive(partner_hb);
	easyCAN(FALSE, 0, ACS_AXIS_CTRL_RESPONSE_ID, 1, (unsigned char *)active_hb);
}

//_____ Temperature Function Wrappers __________________________________________________
void ACS_ADT7320_WRITE_CONFIGURATION(U8 configuration)
{
	xSemaphoreTake(temp_semaphore, portMAX_DELAY);
	PORTB &= ~(ACS_ADT7320_SS_PIN);
	ADT7320_WRITE_CONFIGURATION(configuration);
	PORTB |= ACS_ADT7320_SS_PIN;
	xSemaphoreGive(temp_semaphore);
}

U8 ACS_ADT7320_READ_CONFIGURATION()
{
	U8 config;
	xSemaphoreTake(temp_semaphore, portMAX_DELAY);
	PORTB &= ~(ACS_ADT7320_SS_PIN);
	config = ADT7320_READ_CONFIGURATION();
	PORTB |= ACS_ADT7320_SS_PIN;
	xSemaphoreGive(temp_semaphore);
	return config;
}

U32 ACS_ADT7320_READ_TEMPERATURE()
{
	U32 temp;
	xSemaphoreTake(temp_semaphore, portMAX_DELAY);
	PORTB &= ~(ACS_ADT7320_SS_PIN);
	temp = ADT7320_READ_STATUS();
	PORTB |= ACS_ADT7320_SS_PIN;
	xSemaphoreGive(temp_semaphore);
	return temp;
}



//_____ Misc. Functions __________________________________________________

void watchdogTimerReset()
{
//Watchdog timer reset task, do not run this task when connected with a debugger or the chip will keep reseting itself when you pause.
	
	for(;;)
	{
		wdt_reset();
		vTaskDelay(1900);
	}
}

//_____ RTOS Hooks __________________________________________________

void vApplicationIdleHook( void )
{
		sleep_cpu();
}

void vApplicationTickHook( void )
{
	
}

void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName )
{
	
}

//_____ Interrupt Service Routines __________________________________________________

ISR(X1_FAULT_INT_VECT)
{	
	U8 faults_active = 0;
	
	if(faults_active == 0){
		PORTF &= ~(ACS_nFAULT);
	}else{
		PORTF |= ACS_nFAULT;
	}
}
//These statements alias the rest of the hb interrupts to the function above
ISR(X2_FAULT_INT_VECT, ISR_ALIASOF(X1_FAULT_INT_VECT));
ISR(Y1_FAULT_INT_VECT, ISR_ALIASOF(X1_FAULT_INT_VECT));
ISR(Y2_FAULT_INT_VECT, ISR_ALIASOF(X1_FAULT_INT_VECT));
ISR(Z1orZ2_FAULT_INT_VECT, ISR_ALIASOF(X1_FAULT_INT_VECT));

ISR(OVERTEMP_INT_VECT)
{
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	xSemaphoreTakeFromISR(temp_semaphore, &xHigherPriorityTaskWoken);
	PORTB &= ~(ACS_ADT7320_SS_PIN);
	temperature = ADT7320_READ_TEMPERATURE();
	PORTB |= ACS_ADT7320_SS_PIN;
	xSemaphoreGiveFromISR(temp_semaphore, &xHigherPriorityTaskWoken);
	
	if (temperature >= THIGH){
		easyCAN(TRUE, 0, ACS_OVRTEMP_ALERT_ID, 0, NULL);
		PORTF |= ACS_OVRTMP;
	}else{
		PORTF &= ~(ACS_OVRTMP);
	}
}

