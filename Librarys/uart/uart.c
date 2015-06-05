//******************************************************************************
//! @file $RCSfile: uart.c,v $
//!
//! @todo
//! @bug
//******************************************************************************

//_____ I N C L U D E S ________________________________________________________
#include "FreeRTOSConfig.h"
#include "config.h"
#include "uart.h"

//_____ D E F I N I T I O N S __________________________________________________

//_____ D E C L A R A T I O N S ________________________________________________
#if (USE_UART == BOTH_UART)
    U8 uart_selected;
#endif

//_____ F U N C T I O N S ______________________________________________________

#if ((UART_BAUDRATE == VARIABLE_UART_BAUDRATE))
//------------------------------------------------------------------------------
//  @fn uart_set_baudrate
//!
//! This function programs the UBRRH register of both the UARTs from the
//! value passed as parameter or from an automatic timing detection if
//! there is an 'U' character received on the serial link. This function
//! is called by "Uart_set_baudrate(baudrate)" macro.
//! The function rounds to nearest the division result (i.e. FOSC/BAUDRATE).
//!
//! @warning Lower limit is 300 Bauds per MHz 
//!		(ex: at 8MHz, lower limit is 2400 Bauds).
//!          
//!          Writting UBBR_L triggers an immediate update of the baud
//!          rate prescaler.
//!     Note the role of SOF_DETECTION !
//!
//! @param  !=0: baudrate value
//!         
//!
//! @return Baudrate Status
//!         ==0: baudrate configuration failed
//!         ==1: baudrate performed
//------------------------------------------------------------------------------
U8 uart_set_baudrate (U32 baudrate)
{
    U16 t_meas_4;
    if (baudrate == 0)
    {
		 return 0;
    }
	else
	{
		// UBRRN Value (rounded to nearest division) with U2Xn=1
		t_meas_4 = (U16)(( (((((U32)FOSC*1000)<<1)/((U32)baudrate*8))+1)>>1)-1);
	}
	
    #if (USE_UART ==  UART_0) //use UART0
        UBRR0H  = (U8)((t_meas_4)>>8);
        UBRR0L  = (U8) (t_meas_4)    ;
        UCSR0A |=  (1<<U2X0);
		
    #elif (USE_UART ==  UART_1)//use UART1
        UBRR1H  = (U8)((t_meas_4)>>8);
        UBRR1L  = (U8) (t_meas_4)    ;
        UCSR1A |=  (1<<U2X1);
		
    #elif (USE_UART ==  BOTH_UART)//use Both UARTs
        if (uart_selected == UART_0)
        {
            UBRR0H  = (U8)((t_meas_4)>>8);
            UBRR0L  = (U8) (t_meas_4)    ;
            UCSR0A |=  (1<<U2X0);
        }
        if (uart_selected == UART_1)
        {
            UBRR1H  = (U8)((t_meas_4)>>8);
            UBRR1L  = (U8) (t_meas_4)    ;
            UCSR1A |=  (1<<U2X1);
        }
    #   else
    #       error USE_UART definition is not referenced in "uart.h" file
    #endif
    
    return 1;
}

#endif  // #endif of '((UART_BAUDRATE == ...'


//------------------------------------------------------------------------------
//  @fn uart_rx_get_3_data
//!
//! This function returns the UART pin value sampled 3 times.
//!
//! @warning none
//!
//! @param  none
//!
//! @return UART pin value sampled 3 times
//------------------------------------------------------------------------------
Bool uart_rx_get_3_data (void)
{
U8 u8_temp = 0;

#if ((USE_UART == UART_0) || (USE_UART == UART_1))
    u8_temp = Uart_rx_get_3_data();
    
#elif (USE_UART ==  BOTH_UART)
    if (uart_selected == UART_0)
    {
        u8_temp = (UART_0_PORT_IN  & UART_0_PORT_IN  & UART_0_PORT_IN  & (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN;
    }
    if (uart_selected == UART_1)
    {
        u8_temp = (UART_1_PORT_IN  & UART_1_PORT_IN  & UART_1_PORT_IN  & (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN;
    }
#   else
#       error USE_UART definition is not referenced in "uart.h" file
#endif

    return u8_temp;
}


//------------------------------------------------------------------------------
//  @fn uart_rx_get_data
//!
//! This function returns the UART pin value.
//!
//! @warning none
//!
//! @param  none
//!
//! @return UART pin value
//------------------------------------------------------------------------------
Bool uart_rx_get_data (void)
{
U8 u8_temp = 0;

#if ((USE_UART == UART_0) || (USE_UART == UART_1))
    u8_temp = Uart_rx_get_data();
    
#elif (USE_UART ==  BOTH_UART)
    if (uart_selected == UART_0)
    {
        u8_temp = (UART_0_PORT_IN  & (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN;
    }
    if (uart_selected == UART_1)
    {
        u8_temp = (UART_1_PORT_IN  & (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN;
    }
#   else
#       error USE_UART definition is not referenced in "uart.h" file
#endif

    return u8_temp;
}
//------------------------------------------------------------------------------
//  @fn uart_init
//!
//! UART peripheral initialization. Reset the UART, initialize the uart
//! mode, initialize the baudrate and enable the UART peripheral.
//!
//! @warning If autobaud, only one character is useful. If autobaud, one
//!          16-bit Timer is necessary.
//!
//! @param  Mode (c.f. predefined modes in "uart.h" file)
//!         Baudrate (for fixed baudrate this param is not used)
//!
//! @return Baudrate Status
//!         ==0: research of timing failed
//!         ==1: baudrate performed
//!
//------------------------------------------------------------------------------
U8 uart_init (U8 mode, U32 baudrate)
{
	Uart_clear();       // Flush, Disable and Reset UART
	if (Uart_set_baudrate(baudrate) == 0) return 0;  //!<  c.f. macro in "uart.h"
	Uart_hw_init(mode);     //!<  c.f. macro in "uart.h"
	Uart_enable();          //!<  c.f. macro in "uart.h"
	return (1);
}

//------------------------------------------------------------------------------
//  @fn uart_test_hit
//!
//! Check if something has been received on the UART peripheral.
//!
//! @warning none
//!
//! @param  none
//!
//! @return Baudrate Status
//!         ==0: Nothing has been received
//!         ==1: A character has been received
//!
//------------------------------------------------------------------------------
U8 uart_test_hit (void)
{
	return (Uart_rx_ready());
}

//------------------------------------------------------------------------------
//  @fn uart_putchar
//!
//! Send a character on the UART peripheral.
//!
//! @warning none
//!
//! @param  character to send
//!
//! @return character sent
//!
//------------------------------------------------------------------------------
U8 uart_putchar (U8 ch)
{
	while(!Uart_tx_ready());
	Uart_set_tx_busy();     // Set Busy flag before sending (always)
	Uart_send_byte(ch);
	return (ch);
}

//------------------------------------------------------------------------------
//  @fn uart_getchar
//!
//! Get a character from the UART peripheral.
//!
//! @warning none
//!
//! @param  none
//!
//! @return read (received) character on the UART
//!
//------------------------------------------------------------------------------
U8 uart_getchar (void)
{
	U8 ch;

	while(!Uart_rx_ready());
	ch = Uart_get_byte();
	Uart_ack_rx_byte();
	return ch;
}














