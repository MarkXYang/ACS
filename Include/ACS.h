/*
 * ACS.h
 *
 * Created: 10/09/2014
 *  Author: Brad, Kris
 *
 *	
 *	Intended to be the singular include file for the ACS, to be used on all subsystems.
 *
 *	
 */ 

#ifndef _ACS_H_
#define _ACS_H_

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
#include <avr/sleep.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

//AT90CAN include files
#include "config.h"
#include "can_lib.h"
#include "compiler.h"

//Ecosat2 include files
#include "Ecosat2_CANBus.h"
#include "Ecosat2_messages.h"
#include "../i2c/i2cmaster.h"

//Subsystem feature includes
#include "degauss.h"

//_____ D E F I N I T I O N S _____________________________________________________________________

/**********************************Pin and Byte constants********************************************************/
//H-Bridges
#define DRV8830_SCL_PIN							(1<<PD0)
#define DRV8830_SDA_PIN							(1<<PD1)

#define X1_FAULT_PIN							(1<<PE7)
#define X2_FAULT_PIN							(1<<PE6)
#define Y1_FAULT_PIN							(1<<PE5)
#define Y2_FAULT_PIN							(1<<PE4)
#define Z1orZ2_FAULT_PIN						(1<<PD3)

#define ADT7320_OVERTEMP_PIN					(1<<PD2)

#define ACS_ADT7320_SS_PIN						(1<<PB0)


//Interrupt configuration
#define ACS_EICRA								(U8)((0b01<<ISC30) | (0b01<<ISC20))
#define ACS_EICRB								(U8)((0b01<<ISC70) | (0b01<<ISC60) | (0b01<<ISC50) | (0b01<<ISC40))
#define ACS_EIMSK								(U8)(ADT7320_OVERTEMP_PIN|X1_FAULT_PIN|X2_FAULT_PIN|Y1_FAULT_PIN|Y2_FAULT_PIN|Z1orZ2_FAULT_PIN)

//Direct alert pins
#define ACS_nFAULT								(1<<PF2)
#define ACS_OVRTMP								(1<<PF3)

/**********************************Interrupt vector macros**********************************************/
//This section defines the various interrupt vectors that may be required by peripherals.

#define X1_FAULT_INT_VECT						INT7_vect
#define X2_FAULT_INT_VECT						INT6_vect
#define Y1_FAULT_INT_VECT						INT5_vect
#define Y2_FAULT_INT_VECT						INT4_vect
#define Z1orZ2_FAULT_INT_VECT					INT3_vect
#define OVERTEMP_INT_VECT						INT2_vect

/**********************************Indecies, Addresses, Offsets, and Constans********************************************************/
//H-bridge indecies (used as hb parameter)
#define	X1										0
#define	X2										1
#define	Y1										2
#define	Y2										3
#define	Z1										4
#define	Z2										5

#define TWI_W								0x00
#define TWI_R								0x01

//HB register sub-addresses
#define HB_CONTROL_SUBADDRESS				0x00
#define HB_FAULT_SUBADDRESS					0x01

//Control modes:
#define COAST								0b00
#define REVERSE								0b01
#define FORWARD								0b10
#define BRAKE								0b11

//Control offsets
#define IN10_OFFSET							0
#define VSET_OFFSET							2

//Fault offsets
#define FAULT_OFFSET						1
#define	OCP_OFFSET							2
#define	UVLO_OFFSET							3
#define	OTS_OFFSET							4
#define CLEAR_OFFSET						7

#define MAX_AXIS_COMMAND_DURATION			255


//Temperature Sensor

#define ACS_SPI_CONFIG_BYTE						(U8)(SPI_MASTER|SPI_MSB_FIRST|SPI_DATA_MODE_3|SPI_CLKIO_BY_32)
#define ADT7320_CONFIG_BYTE						(U8)(0b01<<ADT7320_CONFIGURATION_FAULTQUEUE_offset| 0b0<<ADT7320_CONFIGURATION_CTPOLARITY_offset | 0b0<<ADT7320_CONFIGURATION_INTPOLARITY_offset | 0b1<<ADT7320_CONFIGURATION_INTCTMODE_offset | 0b10 << ADT7320_CONFIGURATION_OPERATIONMODE_offset | 0b1<<ADT7320_CONFIGURATION_RESOLUTION_offset)
#define THIGH									0x2000 //64 degrees celcius
#define TLOW									0x0500 //10 degrees celsius
#define TCRIT									0x4980 //147 degrees celsius
#define THYST									0x05   //+/-5 degrees celcius

//_____ Tasks __________________________________________________
void acs_messageHandler();

void acs_axisController();

//_____ CAN Responses __________________________________________________
void acs_status_fault_response();

void acs_status_temp_response();

void acs_degauss_response(U8* packet_data);

void acs_hb_enable_response();

//_____ Initializations __________________________________________________
void acs_init();

//_____ H-Bridge Functions __________________________________________________
int get_hb_write_address(int hb);

int get_hb_read_address(int hb);

void set_control_register(int hb, unsigned char control_byte);
//Sets the control using a command byte

unsigned char get_fault(int hb);
//gets the fault register of the given h-bridge

void clear_fault(int hb);
//clears the fault register of the given h-bridge. Returns 1 if error.

void magnetorquer_off_callback(xTimerHandle pxTimer);
//Callback function used to turn off the magnetorquer specified by the TimerID

//_____ Temperature Function Wrappers __________________________________________________

void ACS_ADT7320_WRITE_CONFIGURATION(U8 configuration);

U8 ACS_ADT7320_READ_CONFIGURATION();

U8 ACS_ADT7320_READ_STATUS();

U32 ACS_ADT7320_READ_TEMPERATURE();


//_____ Misc. Functions __________________________________________________
void watchdogTimerReset();

#endif
