/*
 * ACS.c
 *
 * Created: 10/09/2014
 *  Author: Brad
 *
 *	This file is intended to combine all of the work done by Kris and Brad into one object file to be used by all subsystems.
 *	Currently it includes implementation for nFault and PWM, and requires some debugging.
 *
 *	
 */ 

//_____ I N C L U D E S ___________________________________________________________________________
#include "ACS.h"

//_____ I M P L E M E N T A T I O N S _____________________________________________________________
/*
#################################################
Global Variables
#################################################
*/
	
//Timer handles for callback function that stops the appropriate timer
xTimerHandle stopTimer[NUM_HB];
	
//indecies 1 through 6 hold FAULT register values from corresponding h-bridge
//index 0 contains the fault bits status of h-bridges as either 0 or 1
U8 hb_faults[NUM_HB];

//temperature global variable
U32 temperature;

/*
#################################################
		Initializations
#################################################
*/

void hb_init(void)
{
	PORTD |= 0b00000111;
	DDRD = 0b00000011;
	i2c_init();
	//DDRD |= 0b00000100;
  //Oh for the love of Christ
	/*
	//set data direction registers
	DDRD &= ~(1<<PD1) & ~(1<<PD0);
	DDRE &= ~(1<<PE7);
	DDRA = 0xFF;
	
	DDRA = 0; // 00000000
	DDRA = (0xF<<PA4); // 11110000
	
	DDRA |= 0b00000110; //11110110
	
	
	//enable pull-up resistors
	PORTD |= 1<<PD1 | 1<<PD0;
	PORTE |= 1<<PE7;
	
	
	//interrupts set to trigger on any logic change for INT1, INT0
	EICRA |= (1<<ISC00) | (1<<ISC10);
	EICRA &= (~(1<<ISC01)) & (~(1<<ISC11));
	//for INT7. This is a synchronous interrupt port, and thus a very short duration logic change may go undetected

	EICRB |= (1<<ISC70);
	EICRB &= ~(1<<ISC71);
	
	//enables these interrupts
	EIMSK |= (1<<INT7) | (1<<INT1) | (1<<INT0); 
	*/
}


void power_save_init(void)
{	
	//clock source select
	ASSR &= (0<<AS2);
	//select idle mode as the used sleep mode
	SMCR &= (0<<SM0) | (0<<SM1) | (0<<SM2);
	//enable the usage of the sleep mode
	SMCR |= (1<<SE);
	//disable ADC, analog comparator
	ADCSRA &= (0<<ADEN);
	ACSR |= (1<<ACD);
	
	//The brown out detector, JTAG and on-chip debugging fuses must be disabled in the fuses section of the device programming window in the final version!!
}
/*
void degaussing_table_init()
{
	U32 final_index = (int)(-log(VOLTAGE_THRESHOLD/MAX_VOLTAGE)/(LAMBDA*TWI_PERIOD));
	float omega = 6.283185/(TWI_PERIOD*PERIOD_BYTES);
	
	float ideal_voltage_i;
	float error_i = 0;
	float effected_voltage_i;
	
	U8 vset_i;
	U8 mode_i;
	U8 control_i;
	for(int i = 0; i <= final_index; i++){
		ideal_voltage_i = MAX_VOLTAGE * exp(-LAMBDA*TWI_PERIOD*i)*cos(omega*TWI_PERIOD*i);

		if(ideal_voltage_i >= MIN_VOLTAGE){
			if(error_i < -delta_V){
				vset_i = Min(ceil(ideal_voltage_i/delta_V), MAX_VOLTAGE);

			}
			else{
				vset_i = Max(floor(ideal_voltage_i/delta_V),MIN_VSET) | (FORWARD << IN10_OFFSET);
			}
			
			mode_i = FORWARD;
		}
		else if(ideal_voltage_i >= 0){
			if(error_i < -MIN_VOLTAGE){
				vset_i = MIN_VSET;
				mode_i = FORWARD;
			}
			else{
				vset_i = 0;
				mode_i = BRAKE;
			}
		}
		else if(ideal voltage > -MIN_VOLTAGE){
			if(error_i < MIN_VOLTAGE){
				vset_i = 0;
				mode = BRAKE;
			}
			else{
				vset_i = MIN_VSET;
				mode_i = REVERSE;
			}
		}
		else {
			if(error_i < delta_V){
				vset_i = Max(ceil(ideal_voltage_i/delta_V),MIN_VSET);
			}
			else{
				vset_i = Min(floor(ideal_voltage_i/delta_V),MAX_VSET);
			}
			mode = REVERSE;
		}
		control_i = (vset_i | mode << IN10_OFFSET);
		effected_voltage_i = vset_i * delta_V;
		error_i += effected_voltage_i - ideal_i;
	
		
	}
}

*/

	
}

/*
#################################################
		H-Bridge Functions
#################################################
*/


unsigned char drive_mag(int hb, int duration, int mV, int mode)
{
	U8 error_bit;
	
	U8 vset = (int)(mV/delta_mV);
	
	if(vset < MIN_VSET && vset != 0 || vset > MAX_VSET){
		error_bit |= 1;
	}
	
	set_control_register(hb, (vset << VSET_OFFSET) | (mode << IN10_OFFSET));
	
	//How to implement duration?
	//stopTimer[hb] = xTimerCreate((char *)hb, 1, pdFALSE, ( void * )hb, magnetorquer_off_callback);
}

void set_control_register(int hb, unsigned char control_byte)
{
	i2c_start(0xC0 +2*hb+TWI_W);
	i2c_write(HB_CONTROL_SUBADDRESS);
	i2c_write(control_byte);
	i2c_stop();
}

unsigned char get_fault(hb)
{
	unsigned char fault_byte;
	i2c_start_wait(0xC0 +2*hb + TWI_W);
	i2c_write(HB_FAULT_SUBADDRESS);
	i2c_rep_start(0xC0 + 2*hb + TWI_R);
	fault_byte = i2c_readNak();
	i2c_stop();
	return fault_byte;
}

unsigned char clear_fault(int hb)
{
	unsigned char error_bit;
	error_bit |= i2c_start(get_hb_write_address(hb));
	error_bit |= i2c_write(HB_FAULT_SUBADDRESS);
	error_bit |= i2c_write(1<<CLEAR);
	i2c_stop();
	return error_bit;
}

unsigned char magnetorquer_off_callback(xTimerHandle pxTimer)
{
	unsigned char error_bit;
	error_bit |= i2c_start(get_hb_write_address(pvTimerGetTimerID(pxTimer)));
	error_bit |= i2c_write(HB_CONTROL_SUBADDRESS);
	error_bit |= i2c_write((MIN_VSET<<VSET_OFFSET)|(COAST<<MODE_OFFSET));
	i2c_stop();
	return error_bit;
}

int get_hb_write_address(int hb){return (0xC0 + hb*2);}

int get_hb_read_address(int hb){return (0xC1 + hb*2);}

/*
#################################################
		Temperature Functions
#################################################
*/
void temp_task()
{
	U8 temp_msg[2];
	
	for(;;){
		temperature = ADT7320_READ_TEMPERATURE();
		vTaskDelay(2000);
	}
	
}

/*
#################################################
		Interrupt Service Routines
#################################################
*/

/*
ISR(INT7_vect)
{
	/*
	This ISR handles the nfault conditions and send a message to the OBC over the CAN bus.
	*/
	/*
	
	signed portBASE_TYPE xHigherPriorityTaskWoken;
	if(xHigherPriorityTaskWoken){
		taskYIELD();
	}
	
	easyCAN(TRUE, 0, ACS_HB_FAULT_ID, 6, hb_faults);
}

ISR(INT6_vect, ISR_ALIASOF(INT7_vect));
ISR(INT5_vect, ISR_ALIASOF(INT7_vect));
ISR(INT4_vect, ISR_ALIASOF(INT7_vect));
ISR(INT3_vect, ISR_ALIASOF(INT7_vect));
*/