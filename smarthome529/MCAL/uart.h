/*
 * uart.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef UART_H_
#define UART_H_
#include <avr/io.h>

#define buffer_length  20
#define CPU_OSC 8000000UL
#define rx_enable  (UCSRB|=(1<<RXEN))
#define tx_enable  (UCSRB|=(1<<TXEN))
void uart_init_tx(unsigned int baud);
void uart_init_rx(unsigned int baud);
void uart_transmit(unsigned char data);
 char uart_recieve();
void send_string_to_buffer(char* str);
void send_char_to_buffer(char ch);
void transmit_update();
void recieve_char(char data );
void recieve_update();
void uart_transmit_string(char* str);
unsigned char uart_recie(unsigned char*ss);
#endif /* UART_H_ */
