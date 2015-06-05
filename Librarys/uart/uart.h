//******************************************************************************
//! @file $RCSfile: uart.h,v $
//!
//!
//! @todo
//! @bug
//******************************************************************************

#ifndef _UART_H_
#define _UART_H_

//_____ I N C L U D E S ________________________________________________________

#include "FreeRTOSConfig.h" //used for configCPU_CLOCK_HZ define
#include <stdarg.h>
#include "config.h"
//_____ D E F I N I T I O N S __________________________________________________

#ifndef configCPU_CLOCK_HZ	
#  error  You must define configCPU_CLOCK_HZ in "FreeRTOSConfig.h" file
#endif
    // ----------
#ifndef VARIABLE_UART_BAUDRATE      // UART BaudRate can change during session
#define VARIABLE_UART_BAUDRATE   -1
#endif
    // ----------
#ifndef UART_BAUDRATE
#define UART_BAUDRATE   VARIABLE_UART_BAUDRATE
#endif
    /* ---------- BAUDRATE Examples ----------
    if configCPU_CLOCK_HZ == 16000        //!< Fclkio = 16 MHz
         UBRR_1K2   0x0680     //!< --   1k2 bps, error= 0.0%,
         UBRR_2K4   0x0340     //!< --   2k4 bps, error= 0.0%, U2Xn=1
         UBRR_4K8   0x01A0     //!< --   4k8 bps, error=-0.1%, U2Xn=1
         UBRR_9K6   0x00CF     //!< --   9k6 bps, error= 0.2%, U2Xn=1
         UBRR_14K4  0x008A     //!< --  14k4 bps, error=-0.1%, U2Xn=1
         UBRR_19K2  0x0067     //!< --  19k2 bps, error= 0.2%, U2Xn=1
         UBRR_28K8  0x0044     //!< --  28k8 bps, error= 0.6%, U2Xn=1
         UBRR_38K4  0x0033     //!< --  38k4 bps, error= 0.2%, U2Xn=1
         UBRR_57K6  0x0022     //!< --  57k6 bps, error=-0.8%, U2Xn=1
         UBRR_76K8  0x0019     //!< --  76k8 bps, error= 0.2%, U2Xn=1
         UBRR_115K2 0x0010     //!< -- 115k2 bps, error= 2.1%, U2Xn=1
    if configCPU_CLOCK_HZ == 12000        //!< Fclkio = 12 MHz
         UBRR_1K2   0x04E0     //!< --   1k2 bps, error= 0.0%, U2Xn=1
         UBRR_2K4   0x0270     //!< --   2k4 bps, error= 0.0%, U2Xn=1
         UBRR_4K8   0x0138     //!< --   4k8 bps, error=-0.2%, U2Xn=1
         UBRR_9K6   0x009B     //!< --   9k6 bps, error= 0.2%, U2Xn=1
         UBRR_14K4  0x0067     //!< --  14k4 bps, error= 0.2%, U2Xn=1
         UBRR_19K2  0x004D     //!< --  19k2 bps, error= 0.2%, U2Xn=1
         UBRR_28K8  0x0033     //!< --  28k8 bps, error= 0.2%, U2Xn=1
         UBRR_38K4  0x0026     //!< --  38k4 bps, error= 0.2%, U2Xn=1
         UBRR_57K6  0x0019     //!< --  57k6 bps, error= 0.2%, U2Xn=1
         UBRR_76K8  0x0013     //!< --  76k8 bps, error=-2.5%, U2Xn=1
         UBRR_115K2 0x000C     //!< -- 115k2 bps, error= 0.2%, U2Xn=1
    if configCPU_CLOCK_HZ == 8000         //!< Fclkio = 8 MHz
         UBRR_1K2   0x0340     //!< --   1k2 bps, error= 0.0%, U2Xn=1
         UBRR_2K4   0x01A0     //!< --   2k4 bps, error=-0.1%, U2Xn=1
         UBRR_4K8   0x00CF     //!< --   4k8 bps, error= 0.2%, U2Xn=1
         UBRR_9K6   0x0067     //!< --   9k6 bps, error= 0.2%, U2Xn=1
         UBRR_14K4  0x0044     //!< --  14k4 bps, error= 0.6%, U2Xn=1
         UBRR_19K2  0x0033     //!< --  19k2 bps, error= 0.2%, U2Xn=1
         UBRR_28K8  0x0022     //!< --  28k8 bps, error=-0.8%, U2Xn=1
         UBRR_38K4  0x0019     //!< --  38k4 bps, error= 0.2%, U2Xn=1
         UBRR_57K6  0x0010     //!< --  57k6 bps, error= 2.1%, U2Xn=1
         UBRR_76K8  0x000C     //!< --  76k8 bps, error= 0.2%, U2Xn=1
         UBRR_115K2 0x0008     //!< -- 115k2 bps, error=-3.5%, U2Xn=1   */

    // ----------
#ifndef UART_0               // UART 0 Defintion
#define UART_0      0x00
#endif
#ifndef UART_1               // UART 1 Defintion
#define UART_1      0x01
#endif
#ifndef BOTH_UART            // Both the UARTs Defintion
#define BOTH_UART   0xFF
#endif
#ifndef NO_UART              // No UART Defintion (do not use it in "config.h")
#define NO_UART     0xFE
#endif
    // ----------
#ifndef USE_UART
#       error You must define USE_UART to UART_0 or UART_1 or BOTH_UART

#   elif USE_UART == UART_0
    //!< ==============================
    //!< ------ Only UART_0 used ------
    //!< ==============================
        // Macros
#       define Uart_set_ubrr(baudrate)	{ UBRR0H = (U8)((((((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)>>8); \
                                         UBRR0L = (U8)(( (((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)    ; \
                                         UCSR0A |=  (1<<U2X0); }
#       define Uart_clear()				{ UCSR0B=0; UCSR0C=UDR0; UCSR0A=0x40; UCSR0C=0x06; UBRR0H=0; UBRR0L=0; }
#       define Uart_hw_init(config)		{ UCSR0A |=  (1<<UDRE0);                                    \
                                         UCSR0B &= (~(1<<UCSZ02)); UCSR0B |= config & (1<<UCSZ02); \
                                         UCSR0C  = config & ((3<<UPM00) | (1<<USBS0));             \
                                         UCSR0C |= ((config & (3<<(UCSZ00-1))) << UCSZ00);         }
#       define Uart_enable()			( UCSR0B |=  ((1<<RXEN0) | (1<<TXEN0)) )
#       define Uart_disable()			( UCSR0B &= ~((1<<RXEN0) | (1<<TXEN0)) )
#       define Uart_tx_ready()			(((UCSR0A &   1<<UDRE0)) >> UDRE0 )
#       define Uart_send_byte(ch)		( UDR0 = ch)
#       define Uart_rx_ready()			((UCSR0A & (1<<RXC0)) >> RXC0 )
#       define Uart_get_byte()			( UDR0 )
        // Void
#       define Uart_select(uart_num)//deines which uart module to use
#       define Uart_set_tx_busy()
#       define Uart_ack_rx_byte()
        // Physical UART Definition
#       define UART_0_PORT_IN       (PINE)
#       define UART_0_PORT_DIR      (DDRE)
#       define UART_0_PORT_OUT      (PORTE)
#       define UART_0_INPUT_PIN     (0)
#       define UART_0_OUTPUT_PIN    (1)
#       define Uart_rx_in_pu_on()   { UART_0_PORT_DIR &= ~(1<<UART_0_INPUT_PIN); UART_0_PORT_OUT  |=  (1<<UART_0_INPUT_PIN); }   // RxD in input + pull-up
#       define Uart_rx_in_pu_off()  { UART_0_PORT_DIR &= ~(1<<UART_0_INPUT_PIN); UART_0_PORT_OUT  &= ~(1<<UART_0_INPUT_PIN); }   // RxD in input no pull-up
#       define Uart_rx_get_data()   ((UART_0_PORT_IN  &   (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN )                          // Read RxD pin
#       define Uart_rx_get_3_data() ((UART_0_PORT_IN  & UART_0_PORT_IN  & UART_0_PORT_IN  & (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN )  // Read 3 times RxD pin
#       define Uart_tx_in_pu_on()   { UART_0_PORT_DIR &= ~(1<<UART_0_OUTPUT_PIN); UART_0_PORT_OUT  |=  (1<<UART_0_OUTPUT_PIN); }   // TxD in input + pull-up
#       define Uart_tx_in_pu_off()  { UART_0_PORT_DIR &= ~(1<<UART_0_OUTPUT_PIN); UART_0_PORT_OUT  &= ~(1<<UART_0_OUTPUT_PIN); }   // TxD in input no pull-up
#       define Uart_set_tx()        { UART_0_PORT_DIR |=  (1<<UART_0_INPUT_PIN); UART_0_PORT_OUT |=  (1<<UART_0_INPUT_PIN); }   // TxD in output + set pin
#       define Uart_clear_tx()      { UART_0_PORT_DIR |=  (1<<UART_0_INPUT_PIN); UART_0_PORT_OUT &= ~(1<<UART_0_INPUT_PIN); }   // TxD in output + clear pin

#   elif USE_UART == UART_1
    //!< ==============================
    //!< ------ Only UART_1 used ------
    //!< ==============================
        // Macros
#       define Uart_set_ubrr(baudrate) { UBRR1H = (U8)((((((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)>>8); \
                                         UBRR1L = (U8)(( (((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)    ; \
                                         UCSR1A |=  (1<<U2X1); }
#       define Uart_clear()            { UCSR1B=0; UCSR1C=UDR1; UCSR1A=0x40; UCSR1C=0x06; UBRR1H=0; UBRR1L=0; }
#       define Uart_hw_init(config)    { UCSR1A |=  (1<<UDRE1);                                    \
                                         UCSR1B &= (~(1<<UCSZ12)); UCSR0B |= config & (1<<UCSZ12); \
                                         UCSR1C  = config & ((3<<UPM10) | (1<<USBS1));             \
                                         UCSR1C |= ((config & (3<<(UCSZ10-1))) << UCSZ10);         }
#       define Uart_enable()           ( UCSR1B |=  ((1<<RXEN1) | (1<<TXEN1)) )
#       define Uart_disable()          ( UCSR1B &= ~((1<<RXEN1) | (1<<TXEN1)) )
#       define Uart_tx_ready()        (((UCSR1A &   1<<UDRE1)) >> UDRE1 )
#       define Uart_send_byte(ch)      ( UDR1 = ch)
#       define Uart_rx_ready()         ((UCSR1A & (1<<RXC1)) >> RXC1 )
#       define Uart_get_byte()         ( UDR1 )
        // Void
#       define Uart_select(uart_num)
#       define Uart_set_tx_busy()
#       define Uart_ack_rx_byte()
        // Physical part, pull-up on Rx/Tx
#       define UART_1_PORT_IN       (PIND)
#       define UART_1_PORT_DIR      (DDRD)
#       define UART_1_PORT_OUT      (PORTD)
#       define UART_1_INPUT_PIN     (2)
#       define UART_1_OUTPUT_PIN    (3)
#       define Uart_rx_in_pu_on()   { UART_1_PORT_DIR &= ~(1<<UART_1_INPUT_PIN); UART_1_PORT_OUT  |=  (1<<UART_1_INPUT_PIN); }   // RxD in input + pull-up
#       define Uart_rx_in_pu_off()  { UART_1_PORT_DIR &= ~(1<<UART_1_INPUT_PIN); UART_1_PORT_OUT  &= ~(1<<UART_1_INPUT_PIN); }   // RxD in input no pull-up
#       define Uart_rx_get_data()   ((UART_1_PORT_IN  &   (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN )                          // Read RxD pin
#       define Uart_rx_get_3_data() ((UART_1_PORT_IN  & UART_1_PORT_IN  & UART_1_PORT_IN  & (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN )  // Read 3 times RxD pin
#       define Uart_tx_in_pu_on()   { UART_1_PORT_DIR &= ~(1<<UART_1_OUTPUT_PIN); UART_1_PORT_OUT  |=  (1<<UART_1_OUTPUT_PIN); }   // TxD in input + pull-up
#       define Uart_tx_in_pu_off()  { UART_1_PORT_DIR &= ~(1<<UART_1_OUTPUT_PIN); UART_1_PORT_OUT  &= ~(1<<UART_1_OUTPUT_PIN); }   // TxD in input no pull-up
#       define Uart_set_tx()        { UART_1_PORT_DIR |=  (1<<UART_1_INPUT_PIN); UART_1_PORT_OUT |=  (1<<UART_1_INPUT_PIN); }   // TxD in output + set pin
#       define Uart_clear_tx()      { UART_1_PORT_DIR |=  (1<<UART_1_INPUT_PIN); UART_1_PORT_OUT &= ~(1<<UART_1_INPUT_PIN); }   // TxD in output + clear pin

#   elif USE_UART  == BOTH_UART
    //!< =====================================
    //!< ------ Both the UARTs are used ------
    //!< =====================================
        extern U8 uart_selected;      // $$$-- EXTERNAL DECLARATION --$$$
        #define Uart_select(uart_num)   (uart_selected=uart_num)
        // Macros
#       define Uart_set_ubrr(baudrate)  ((uart_selected==UART_0)? \
                                            (UBRR0H  = (U8)((((((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)>>8), \
                                             UBRR0L  = (U8)(( (((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)    , \
                                             UCSR0A |=  (1<<U2X0) )\
                                            : \
                                            (UBRR1H  = (U8)((((((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)>>8), \
                                             UBRR1L  = (U8)(( (((((U32)configCPU_CLOCK_HZ)<<1)/((U32)baudrate*8))+1)>>1)-1)    , \
                                             UCSR1A |=  (1<<U2X1) )                                                     )
#       define Uart_clear()             ((uart_selected==UART_0)? \
                                            (UCSR0B=0, UCSR0C=UDR0, UCSR0A=0x40, UCSR0C=0x06, UBRR0H=0, UBRR0L=0) \
                                            : \
                                            (UCSR1B=0, UCSR1C=UDR1, UCSR1A=0x40, UCSR1C=0x06, UBRR1H=0, UBRR1L=0) )
#       define Uart_hw_init(config)     ((uart_selected==UART_0)? \
                                            (UCSR0A |=  (1<<UDRE0),                             \
                                             UCSR0B &= (~(1<<UCSZ02)),                          \
                                             UCSR0B |= config & (1<<UCSZ02),                    \
                                             UCSR0C  = config & ((3<<UPM00) | (1<<USBS0)),      \
                                             UCSR0C |= ((config & (3<<(UCSZ00-1))) << UCSZ00) ) \
                                            : \
                                            (UCSR1A |=  (1<<UDRE1),                                    \
                                             UCSR1B &= (~(1<<UCSZ12)),                          \
                                             UCSR1B |= config & (1<<UCSZ12),                    \
                                             UCSR1C  = config & ((3<<UPM10) | (1<<USBS1)),      \
                                             UCSR1C |= ((config & (3<<(UCSZ10-1))) << UCSZ10) ) )
#       define Uart_enable()            ((uart_selected==UART_0)? \
                                            (UCSR0B |=  ((1<<RXEN0) | (1<<TXEN0))) \
                                            : \
                                            (UCSR1B |=  ((1<<RXEN1) | (1<<TXEN1))) )
#       define Uart_disable()           ((uart_selected==UART_0)? \
                                            (UCSR0B &= ~((1<<RXEN0) | (1<<TXEN0))) \
                                            : \
                                            (UCSR1B &= ~((1<<RXEN1) | (1<<TXEN1))) )
#       define Uart_tx_ready()          ((uart_selected==UART_0)? \
                                            (((UCSR0A &   1<<UDRE0)) >> UDRE0) \
                                            : \
                                            (((UCSR1A &   1<<UDRE1)) >> UDRE1) )
#       define Uart_send_byte(ch)       ((uart_selected==UART_0)? (UDR0 = ch) : (UDR1 = ch) )
#       define Uart_rx_ready()          ((uart_selected==UART_0)? \
                                            ((UCSR0A & (1<<RXC0)) >> RXC0) \
                                            : \
                                            ((UCSR1A & (1<<RXC1)) >> RXC1) )
#       define Uart_get_byte()          ((uart_selected==UART_0)? (UDR0) : (UDR1) )
        // Void
#       define Uart_set_tx_busy()
#       define Uart_ack_rx_byte()
        // Physical part, pull-up on Rx/Tx
#       define UART_0_PORT_IN           (PINE)
#       define UART_1_PORT_IN           (PIND)
#       define UART_0_PORT_DIR          (DDRE)
#       define UART_1_PORT_DIR          (DDRD)
#       define UART_0_PORT_OUT          (PORTE)
#       define UART_1_PORT_OUT          (PORTD)
#       define UART_0_INPUT_PIN         (0)
#       define UART_1_INPUT_PIN         (2)
#       define UART_0_OUTPUT_PIN        (1)
#       define UART_1_OUTPUT_PIN        (3)
#       define Uart_rx_in_pu_on()       ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR &= ~(1<<UART_0_INPUT_PIN), UART_0_PORT_OUT  |=  (1<<UART_0_INPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR &= ~(1<<UART_1_INPUT_PIN), UART_1_PORT_OUT  |=  (1<<UART_1_INPUT_PIN) ) ) // RxD in input + pull-up
#       define Uart_rx_in_pu_off()      ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR &= ~(1<<UART_0_INPUT_PIN), UART_0_PORT_OUT  &= ~(1<<UART_0_INPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR &= ~(1<<UART_1_INPUT_PIN), UART_1_PORT_OUT  &= ~(1<<UART_1_INPUT_PIN) ) ) // RxD in input no pull-up
#       define Uart_rx_get_data()       ((uart_selected==UART_0)? \
                                            ((UART_0_PORT_IN  &   (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN ) \
                                            : \
                                            ((UART_1_PORT_IN  &   (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN ) )                      // Read RxD pin
#       define Uart_rx_get_3_data()     ((uart_selected==UART_0)? \
                                            ((UART_0_PORT_IN  & UART_0_PORT_IN  & UART_0_PORT_IN  & (1<<UART_0_INPUT_PIN)) >> UART_0_INPUT_PIN ) \
                                            : \
                                            ((UART_1_PORT_IN  & UART_1_PORT_IN  & UART_1_PORT_IN  & (1<<UART_1_INPUT_PIN)) >> UART_1_INPUT_PIN ) )    // Read 3 times RxD pin
#       define Uart_tx_in_pu_on()       ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR &= ~(1<<UART_0_OUTPUT_PIN), UART_0_PORT_OUT  |=  (1<<UART_0_OUTPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR &= ~(1<<UART_1_OUTPUT_PIN), UART_1_PORT_OUT  |=  (1<<UART_1_OUTPUT_PIN) ) ) // TxD in input + pull-up
#       define Uart_tx_in_pu_off()      ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR &= ~(1<<UART_0_OUTPUT_PIN), UART_0_PORT_OUT  &= ~(1<<UART_0_OUTPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR &= ~(1<<UART_1_OUTPUT_PIN), UART_1_PORT_OUT  &= ~(1<<UART_1_OUTPUT_PIN) ) ) // TxD in input no pull-up
#       define Uart_set_tx()            ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR |=  (1<<UART_0_INPUT_PIN), UART_0_PORT_OUT |=  (1<<UART_0_INPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR |=  (1<<UART_1_INPUT_PIN), UART_1_PORT_OUT |=  (1<<UART_1_INPUT_PIN) ) ) // TxD in output + set pin
#       define Uart_clear_tx()          ((uart_selected==UART_0)? \
                                            (UART_0_PORT_DIR |=  (1<<UART_0_INPUT_PIN), UART_0_PORT_OUT &= ~(1<<UART_0_INPUT_PIN) ) \
                                            : \
                                            (UART_1_PORT_DIR |=  (1<<UART_1_INPUT_PIN), UART_1_PORT_OUT &= ~(1<<UART_1_INPUT_PIN) ) ) // TxD in output + clear pin
#   else
#       error USE_UART definition is not referenced in "uart.h" file
#endif

    // ---------- Pre-definitions for "Uart_hw_init(config)" macro.
#define CONF_9BIT_NOPAR_1STOP      ( (0<<UPM00) | (0<<USBS0) | (7<<(UCSZ00-1)) )
#define CONF_9BIT_NOPAR_2STOP      ( (0<<UPM00) | (1<<USBS0) | (7<<(UCSZ00-1)) )
#define CONF_9BIT_EVENPAR_1STOP    ( (2<<UPM00) | (0<<USBS0) | (7<<(UCSZ00-1)) )
#define CONF_9BIT_EVENPAR_2STOP    ( (2<<UPM00) | (1<<USBS0) | (7<<(UCSZ00-1)) )
#define CONF_9BIT_ODDPAR_1STOP     ( (3<<UPM00) | (0<<USBS0) | (7<<(UCSZ00-1)) )
#define CONF_9BIT_ODDPAR_2STOP     ( (3<<UPM00) | (1<<USBS0) | (7<<(UCSZ00-1)) )

#define CONF_8BIT_NOPAR_1STOP      ( (0<<UPM00) | (0<<USBS0) | (3<<(UCSZ00-1)) )
#define CONF_8BIT_NOPAR_2STOP      ( (0<<UPM00) | (1<<USBS0) | (3<<(UCSZ00-1)) )
#define CONF_8BIT_EVENPAR_1STOP    ( (2<<UPM00) | (0<<USBS0) | (3<<(UCSZ00-1)) )
#define CONF_8BIT_EVENPAR_2STOP    ( (2<<UPM00) | (1<<USBS0) | (3<<(UCSZ00-1)) )
#define CONF_8BIT_ODDPAR_1STOP     ( (3<<UPM00) | (0<<USBS0) | (3<<(UCSZ00-1)) )
#define CONF_8BIT_ODDPAR_2STOP     ( (3<<UPM00) | (1<<USBS0) | (3<<(UCSZ00-1)) )

#define CONF_7BIT_NOPAR_1STOP      ( (0<<UPM00) | (0<<USBS0) | (2<<(UCSZ00-1)) )
#define CONF_7BIT_NOPAR_2STOP      ( (0<<UPM00) | (1<<USBS0) | (2<<(UCSZ00-1)) )
#define CONF_7BIT_EVENPAR_1STOP    ( (2<<UPM00) | (0<<USBS0) | (2<<(UCSZ00-1)) )
#define CONF_7BIT_EVENPAR_2STOP    ( (2<<UPM00) | (1<<USBS0) | (2<<(UCSZ00-1)) )
#define CONF_7BIT_ODDPAR_1STOP     ( (3<<UPM00) | (0<<USBS0) | (2<<(UCSZ00-1)) )
#define CONF_7BIT_ODDPAR_2STOP     ( (3<<UPM00) | (1<<USBS0) | (2<<(UCSZ00-1)) )

#define CONF_6BIT_NOPAR_1STOP      ( (0<<UPM00) | (0<<USBS0) | (1<<(UCSZ00-1)) )
#define CONF_6BIT_NOPAR_2STOP      ( (0<<UPM00) | (1<<USBS0) | (1<<(UCSZ00-1)) )
#define CONF_6BIT_EVENPAR_1STOP    ( (2<<UPM00) | (0<<USBS0) | (1<<(UCSZ00-1)) )
#define CONF_6BIT_EVENPAR_2STOP    ( (2<<UPM00) | (1<<USBS0) | (1<<(UCSZ00-1)) )
#define CONF_6BIT_ODDPAR_1STOP     ( (3<<UPM00) | (0<<USBS0) | (1<<(UCSZ00-1)) )
#define CONF_6BIT_ODDPAR_2STOP     ( (3<<UPM00) | (1<<USBS0) | (1<<(UCSZ00-1)) )

#define CONF_5BIT_NOPAR_1STOP      ( (0<<UPM00) | (0<<USBS0) | (0<<(UCSZ00-1)) )
#define CONF_5BIT_NOPAR_2STOP      ( (0<<UPM00) | (1<<USBS0) | (0<<(UCSZ00-1)) )
#define CONF_5BIT_EVENPAR_1STOP    ( (2<<UPM00) | (0<<USBS0) | (0<<(UCSZ00-1)) )
#define CONF_5BIT_EVENPAR_2STOP    ( (2<<UPM00) | (1<<USBS0) | (0<<(UCSZ00-1)) )
#define CONF_5BIT_ODDPAR_1STOP     ( (3<<UPM00) | (0<<USBS0) | (0<<(UCSZ00-1)) )
#define CONF_5BIT_ODDPAR_2STOP     ( (3<<UPM00) | (1<<USBS0) | (0<<(UCSZ00-1)) )

//_____ M A C R O S ____________________________________________________________

//------------------------------------------------------------------------------
//! Set UART baudrate.
//! 1) If "UART_BAUDRATE == xxxxx" in "FreeRTOSConfig.h" file, all the UARTs defined
//!    will be set with this value (computation made by the C pre-processor).
//! 2) If "UART_BAUDRATE == VARIABLE_UART_BAUDRATE" in "FreeRTOSConfig.h" file,
//!    this means that the application will set itself the baudrate value
//!    and this value could be changed during the session or/and the UARTs
//!    available into the microcontroller could have their own baudrate.
//! 3) If "UART_BAUDRATE" is not defined in "FreeRTOSConfig.h" file, "UART_BAUDRATE"
//!    will be defined and will be equals to "VARIABLE_UART_BAUDRATE". Then
//!    same behavior than in 2).
//!
//! @warning Write UBBR_H first then UBBR_L last, writting UBBR_L triggers
//!          an immediate update of the baud rate prescaler.
//!
//! The macro "Uart_set_baudrate(baudrate)" rounds to nearest the division
//! result (i.e. configCPU_CLOCK_HZ/BAUDRATE).
//!
//! U2Xnset to 1 for all type of computation.
//!
//------------------------------------------------------------------------------
#if UART_BAUDRATE == VARIABLE_UART_BAUDRATE
#   define Uart_set_baudrate(baudrate) ( uart_set_baudrate(baudrate) )
#else
#   define Uart_set_baudrate(baudrate) ( Uart_set_ubrr(UART_BAUDRATE))
#endif

//_____ D E C L A R A T I O N S ________________________________________________

// (c.f. above) extern char uart_selected;

#if ((UART_BAUDRATE == VARIABLE_UART_BAUDRATE))
//------------------------------------------------------------------------------
//  @fn uart_set_baudrate
//!
//! This function programs the UBRRH register of both the UARTs from the
//! value passed as parameter or from an automatic timing detection if
//! there is some communication (activity) on the serial link. This function
//! is called by "Uart_set_baudrate(baudrate)" macro.
//! The function rounds to nearest the division result (i.e. configCPU_CLOCK_HZ/BAUDRATE).
//!
//! @warning Write UBBR_H first then UBBR_L last, writting UBBR_L triggers
//!          an immediate update of the baud rate prescaler.
//!
//! @param  !=0: baudrate value
//!         ==0: automatic baudrate detection
//!
//! @return Baudrate Status
//!         ==0: baudrate configuration failed
//!         ==1: baudrate performed
//!
extern  U8 uart_set_baudrate(U32 baudrate);
#endif
//##############################################################################
//
//
//
//				FUNCTION DECLARATIONS
//
//
//
//###############################################################################
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
extern  Bool uart_rx_get_3_data (void);

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
extern  Bool uart_rx_get_data (void);

//______________________________________________________________________________
//_____ D E C L A R A T I O N S ________________________________________________

//------------------------------------------------------------------------------
//  @fn uart_init
//!
//! UART peripheral initialization. Reset the UART, initialize the uart
//! mode, initialize the baudrate and enable the UART peripheral.
//!
//! @warning If autobaud, only one character is useful. If autobaud, one
//!          16-bit Timer is necessary.
//!
//! @param  Mode (c.f. predefined modes in "uart_drv.h" file)
//!         Baudrate (for fixed baudrate this param is not used)
//!
//! @return Baudrate Status
//!         ==0: research of timing failed
//!         ==1: baudrate performed
//!
extern U8 uart_init (U8 mode, U32 baudrate);

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
extern U8 uart_test_hit (void);

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
extern U8 uart_putchar (U8 ch);

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
extern U8 uart_getchar (void);

//------------------------------------------------------------------------------
//  @fn uart_put_string
//!
//! Put a data-string on TX UART. The data-string is send up to null
//! character is found.
//!
//! @warning "uart_init()" must be performed before
//!
//! @param Pointer on U8 data-string
//!
//! @return (none)
//!
void uart_put_string (U8 *data_string);

//------------------------------------------------------------------------------
//  @fn uart_mini_printf
//!
//! Minimal "PRINTF" with variable argument list. Write several variables
//! formatted by a format string to a file descriptor.
//! Example:
//! ========
//! { u8_toto = 0xAA;
//!   uart_mini_printf ("toto = %04d (0x%012X)\r\n", u8_toto, u8_toto);
//!   /*   Expected:     toto = 0170 (0x0000000000AA)   &  Cr+Lf       */ }
//!
//! @warning "uart_init()" must be performed before
//!
//! @param argument list
//!
//!     The format string is interpreted like this:
//!        ,---------------,---------------------------------------------------,
//!        | Any character | Output as is                                      |
//!        |---------------+---------------------------------------------------|
//!        |     %c:       | interpret argument as character                   |
//!        |     %s:       | interpret argument as pointer to string           |
//!        |     %d:       | interpret argument as decimal (signed) S16        |
//!        |     %ld:      | interpret argument as decimal (signed) S32        |
//!        |     %u:       | interpret argument as decimal (unsigned) U16      |
//!        |     %lu:      | interpret argument as decimal (unsigned) U32      |
//!        |     %x:       | interpret argument as hex U16 (lower case chars)  |
//!        |     %lx:      | interpret argument as hex U32 (lower case chars)  |
//!        |     %X:       | interpret argument as hex U16 (upper case chars)  |
//!        |     %lX:      | interpret argument as hex U32 (upper case chars)  |
//!        |     %%:       | print a percent ('%') character                   |
//!        '---------------'---------------------------------------------------'
//!
//!     Field width (in decimal) always starts with "0" and its maximum is
//!     given by "DATA_BUF_LEN" defined in "uart_lib.h".
//!        ,----------------------,-----------,--------------,-----------------,
//!        |       Variable       | Writting  |  Printing    |    Comment      |
//!        |----------------------+-----------+--------------|-----------------|
//!        |                      |   %x      | aa           |        -        |
//!        |  u8_xx = 0xAA        |   %04d    | 0170         |        -        |
//!        |                      |   %012X   | 0000000000AA |        -        |
//!        |----------------------+-----------+--------------|-----------------|
//!        | u16_xx = -5678       |   %010d   | -0000005678  |        -        |
//!        |----------------------+-----------+--------------|-----------------|
//!        | u32_xx = -4100000000 |   %011lu  | 00194967296  |        -        |
//!        |----------------------+-----------+--------------|-----------------|
//!        |          -           |   %8x     | 8x           | Writting error! |
//!        |----------------------+-----------+--------------|-----------------|
//!        |          -           |   %0s     | 0s           | Writting error! |
//!        '----------------------'-----------'--------------'-----------------'
//!       
//! Return: 0 = O.K.
//!
#endif  // _UART_H_

