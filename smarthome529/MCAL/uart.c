/*
 * uart.c
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */
#include"uart.h"
#define set_8bits_charatersize  (UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1))
#define select_parity_mode(mode) (UCSRC|=(1<<URSEL)|(1<<UPM1)|(1<<UPM0))
#define  odd_parity    (1)
#define  even_parity   (0)
#define select_stopbit_N(N_bit)   (UCSRC|=(1<<URSEL)|(N_bit<<USBS))
#define  one_stopbit    (0)
#define  two_stopbit    (1)
static unsigned char not_written_yet=0;

void uart_init_tx(unsigned int baud){
	tx_enable;
	(UCSRC&=~(1<<URSEL));

unsigned int prescale=0;
prescale=(((CPU_OSC / (baud * 16UL) ))-1);
UBRRH= (unsigned char)(prescale>>8);
	UBRRL=(unsigned char) prescale;

	set_8bits_charatersize	;//for 8bit

	select_parity_mode(1)	;//odd parity
	select_stopbit_N(0);
}
void uart_init_rx(unsigned int baud){
	rx_enable;
	(UCSRC&=~(1<<URSEL));

unsigned int prescale=0;
prescale=(((CPU_OSC / (baud * 16UL) ))-1);
UBRRH= (unsigned char)(prescale>>8);
	UBRRL=(unsigned char) prescale;

	set_8bits_charatersize	;//for 8bit

	select_parity_mode(1)	;//odd parity
	select_stopbit_N(0);
}
void uart_transmit(unsigned char data){
	while(!(UCSRA &(1<<UDRE)));
	UDR=data;
	while(!(UCSRA & (1<<TXC)));
}
 char uart_recieve(){


	while((UCSRA &(1<<RXC))==0);
	return UDR;
}
static unsigned char buffer [buffer_length];
void send_char_to_buffer(char ch){

	if(not_written_yet<buffer_length){
		buffer [not_written_yet]=ch;
		not_written_yet++;

	}
}
void send_string_to_buffer(char *str){
	unsigned char n=0;
	while(str[n]!='\0'){
		send_char_to_buffer(str[n]);
		n++;
	}
}
	void transmit_update(){
	static	unsigned char waiting=0;
		if(not_written_yet>waiting){
			uart_transmit(buffer[waiting]);
			waiting++;
		}
		else{
			not_written_yet=0;
			waiting=0;

		}
	}
	static unsigned char recieve_buffer[buffer_length];
	static unsigned char recived_wait=0;
void recieve_char(char data ){
	if(recived_wait<buffer_length){

		recieve_buffer[recived_wait]=data;
		recived_wait++;
	}
}
void recieve_update(){

	char data=0;
	data=uart_recieve();
	recieve_char(data);
}
void uart_transmit_string(char* str){
	int i=0;
	char ss;

	while(str[i]!='\0'){

		uart_transmit(str[i]);
		i++;


	}



}
 unsigned char uart_recie(unsigned char*ss){


	while(!((UCSRA>>RXC)&1));
*ss=UDR;
	return *ss;
}
