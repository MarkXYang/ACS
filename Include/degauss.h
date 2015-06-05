/*
 * degauss.h
 *
 * Created: 05/12/2014
 *  Author: Brad Maibach
 *
 *	
 *	
 *
 *	
 */ 

#ifndef _DEGAUSS_H_
#define _DEGAUSS_H_
//_____ I N C L U D E S ________________________________________________________
#include <avr/io.h>
#include <avr/pgmspace.h>

//_____ D E F I N I T I O N S __________________________________________________
#define DEGAUSSING_FINAL_INDEX					32767

//_____ M A C R O S ____________________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________
unsigned char get_controlbyte(int index);


#endif