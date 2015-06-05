/*
 * ADT7320_TemperatureSensor.c
 *
 * Created: 12/02/2014 4:15:34 PM
 *  Author: Cass
 *
 *
 *	This library requires that you drive the correct CS pin around each function call
 *  Please read the ADT7320 Datasheet at http://www.analog.com/static/imported-files/data_sheets/ADT7320.pdf
 *
 */ 

#include "ADT7320_TemperatureSensor.h"
#include "spi_lib.h"


//SPI Send and Get Commands, replace spi library include above as well as the spi_putchar and ADT7320_GETchar functions with the appropriate functions for your firmware.
U8 ADT7320_SEND(char ch){
	return 	spi_putchar(ch);
}

U8 ADT7320_GET(void){ 
	return spi_getchar();
}

U8 ADT7320_READ_STATUS(void){
	char cmdbyte = (char)((1 << ADT7320_RW_offset) & (ADT7320_STATUS_ADDR << ADT7320_ADDR_offset));
	ADT7320_SEND(cmdbyte);
	return ADT7320_GET();
}

U8 ADT7320_READ_CONFIGURATION(void){
	char cmdbyte = (char)((1 << ADT7320_RW_offset) & (ADT7320_CONFIGURATION_ADDR << ADT7320_ADDR_offset));
	ADT7320_SEND(cmdbyte);
	return ADT7320_GET();
}

void ADT7320_WRITE_CONFIGURATION(U8 configuration){
	char cmdbyte = (char)((0 << ADT7320_RW_offset) & (ADT7320_CONFIGURATION_ADDR << ADT7320_ADDR_offset));
	ADT7320_SEND(cmdbyte);
	ADT7320_SEND((char)configuration);
}

U32 ADT7320_READ_TEMPERATURE(void){
	char cmdbyte = (char)((1 << ADT7320_RW_offset) & (ADT7320_TEMPERATURE_VALUE_ADDR << ADT7320_ADDR_offset));
	U32 returnValue = 0;
	ADT7320_SEND(cmdbyte);
	returnValue = ADT7320_GET();
	returnValue = returnValue << 8;
	returnValue |= ADT7320_GET();
	return returnValue;
}