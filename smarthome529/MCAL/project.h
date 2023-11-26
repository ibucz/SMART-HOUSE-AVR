/*
 * project.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */


#include <avr/io.h>
#include <stdio.h>
#define F_CPU 8000000UL			/* Define frequency here its 8MHz */

/* GPIO definitions */
/* GPIO Pins */
#define GPIO_PIN_0              (0)
#define GPIO_PIN_1              (1)
#define GPIO_PIN_2              (2)
#define GPIO_PIN_3              (3)
#define GPIO_PIN_4              (4)
#define GPIO_PIN_5              (5)
#define GPIO_PIN_6              (6)
#define GPIO_PIN_7              (7)

/* GPIO Port Registers redefinition */
#define GPIO_PORTA_DATA         (PORTA)
#define GPIO_PORTA_CONTROL      (DDRA)

#define GPIO_PORTB_DATA         (PORTB)
#define GPIO_PORTB_CONTROL      (DDRB)

#define GPIO_PORTC_DATA         (PORTC)
#define GPIO_PORTC_CONTROL      (DDRC)

#define GPIO_PORTD_DATA         (PORTD)
#define GPIO_PORTD_CONTROL      (DDRD)



/* GPIO direction setting */
#define GPIO_OUT                (1)
#define GPIO_IN                 (0)



/* GPIO port operations */
#define GPIO_InitPort(CONTROL, DIRECTION)   ((CONTROL) = ((DIRECTION)? (~0):(0)))
#define GPIO_WritePort(PORT, DATA)          ((PORT) = (DATA))
#define GPIO_ReadPort(PORT)                 (PORT)

/* GPIO port pin operations */
#define GPIO_Initpin(CONTROL,NPIN,DIRECTION)    ( (CONTROL)  =  (( CONTROL  &  (~(1<<NPIN))  )  |  (DIRECTION<<NPIN) ))
#define GPIO_Writepin(PORT,NPIN,DATA)                ( (PORT)  =   ((    PORT   &  (~(1<<NPIN))  )  |  (DATA<<NPIN) ))
#define GPIO_Readpin(PORT,NPIN)                     (((PORT)&(1<<NPIN))   >>   (NPIN))
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UART registers
#define UART_control_registerA     (UCSRA)
#define UART_control_registerB     (UCSRB)
#define UART_control_registerC     (UCSRC)
#define UART_baudrate_registerH    (UBRRH)
#define UART_baudrate_registerL    (UBRRL)
#define UART_data_register         (UDR)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UART equation of setting  baud rate
#define BAUD_PRESCALE(USART_BAUDRATE) (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//double UART transimission speed
#define double_transmission_speed (UART_control_registerA|=(1<<U2X))
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// enable interupts of UART
#define set_receivecomplete_interupt     (UART_control_registerB|=(1<<RXCIE))
#define set_transmitcomplete_interupt    (UART_control_registerB|=(1<<TXCIE))
#define set_dataregister_empty_interupt  (UART_control_registerB|=(1<<UDRIE))
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//enable receive and transmit of UART
#define set_receive_EN       (UART_control_registerB|=(1<<RXEN))
#define set_transmit_EN      (UART_control_registerB|=(1<<TXEN))
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UUART character size (data size)
#define set_8bits_charatersize      (UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1))
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define select_parity_mode(mode)    (UCSRC|=(1<<URSEL)|(1<<UPM1)|(mode<<UPM0))
#define  odd_parity    (1)
#define  even_parity   (0)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define select_stopbit_N(N_bit)       (UCSRC|=(1<<URSEL)|(N_bit<<USBS))
#define  one_stopbit    (0)
#define  two_stopbit    (1)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define UART_baudrate_registerH_update       (UCSRC&=~(1<<URSEL))      //memory shared issue
// URSEL bit-->  select for selection between two register shared same i/o location
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
