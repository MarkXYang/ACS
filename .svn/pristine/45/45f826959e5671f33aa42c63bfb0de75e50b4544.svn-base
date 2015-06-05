/*
 * Ecosat2_UART.c .c
 *
 * Created: 10/09/2014
 *  Author: Brad Maibach
 *
 *	This is the code written by Cass for the UART. Originally all in the main file, Reorganized by Brad.
 *	
 *
 *	
 */ 

//_____ I N C L U D E S ___________________________________________________________________________
#include "Ecosat2_UART.h"

//_____ D E F I N I T I O N S _____________________________________________________________________

//_____ M A C R O S _______________________________________________________________________________

//_____ I M P L E M E N T A T I O N S _____________________________________________________________
/*
####################################################
		Initializations
####################################################
*/
void UART_INIT_TEMP(void)
{
	//UBRR = 12 at 8MHz clock should be 76.8KHz
	UBRR1H = (U8) 0;
	UBRR1L = (U8) 12;
	UCSR1A |= (1 << U2X1);
	UCSR1B = (1 << RXCIE) | (1 << TXCIE) | (1 << RXEN) | (1 << TXEN);
	UCSR1C = (0 << UMSEL) | (3 << UPM0) | (0 << USBS) | (3 << UCSZ0); //see page 198 of data sheet, 8bit odd parity, 1 stop
	
	/*********************************Create Queues*******************************************************/
	uart_receive_queue = xQueueCreate(24, sizeof(U8)); //queue of bytes received over UART
	uart_transmit_queue = xQueueCreate(5, sizeof(can_format));
	uart_transmit_queue_physical = xQueueCreate(24, sizeof(U8));
}
/*
####################################################
		Tasks
####################################################
*/
void uart_handler()
{
	U8 temp = 0;
	//U8 verificationPacket[11] = {0,0,0,0,0,0,0,0,0,0,0};
	//U8 byteIndex = 0;
	//can_format uart_message;
	//U8 state = UART_WAITING;
	//U8 packetLength = 0;

	for(;;){
		portBASE_TYPE flag = xQueueReceive(uart_receive_queue, (void *) &(temp), portMAX_DELAY);
		if(flag){
			PORTA |= URx;
			
			PORTA &= ~URx;
		}
	}
}

void uart_transmit_handler()
{
	can_format cmd_from_queue;
	//U8 temp;
	for(;;){
		portBASE_TYPE flag = xQueueReceive(uart_transmit_queue, (void *) &(cmd_from_queue), portMAX_DELAY);
		if(flag){
			PORTA |= UTx;
			
			PORTA &= ~UTx;
		}	
	}
}

/*
####################################################
		Other Functions
####################################################
*/
/*
####################################################
		Interrupt Service Routines
####################################################
*/
ISR(USART1_RX_vect)
{
	/*
		Interrupt service routine for the RX interrupt on UART1
	*/
	U8 received_data_uart = UDR1;
	signed portBASE_TYPE xTaskWokenByPost;
	xQueueSendFromISR(uart_receive_queue, (void *) &received_data_uart, &xTaskWokenByPost);
	if( xTaskWokenByPost )
	{
		taskYIELD();
	}
}


ISR(USART1_TX_vect){
	/*
		Interrupt service routine for the TX interrupt on UART1
	*/
	U8 transmit_data_uart = 0;
	signed portBASE_TYPE xTaskWokenByPull;
	if(xQueueReceiveFromISR(uart_transmit_queue_physical, (void *) &(transmit_data_uart), &xTaskWokenByPull))
	{
		UDR1 = transmit_data_uart;
	}
	if( xTaskWokenByPull )
	{
		taskYIELD();
	}
}