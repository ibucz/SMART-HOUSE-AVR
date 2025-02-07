/*
 * DIO.c
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#include"DIO.h"
void set_port_direction(port_ID port,port_DIR direction){
	if(direction==output){
		switch(port){

		case portA:DDRA_reg=0xff   ; break;
		case portB:DDRB_reg=0xff    ; break;
		case portC:DDRC_reg=0xff    ; break;
		case portD:DDRD_reg=0xff    ; break;
		}

		if(direction==input){
				switch(port){

				case portA:DDRA_reg=0x00   ; break;
				case portB:DDRB_reg=0x00    ; break;
				case portC:DDRC_reg=0x00    ; break;
				case portD:DDRD_reg=0x00    ; break;
				}


		}


	}








}
void set_port_value(port_ID port,unsigned char value){


		switch(port){

		case portA:PORTA_reg=value   ; break;
		case portB:PORTB_reg=value   ; break;
		case portC:PORTC_reg=value    ; break;
		case portD:PORTD_reg=value    ; break;
		}







	}





void set_PIN_direction(port_ID port,pin_ID pin,port_DIR direction){
if(port<=portD && pin<=7){
	if(direction==output){
	switch(port){

			case portA:DDRA_reg|= (1<<pin); break;
			case portB:DDRB_reg|= (1<<pin)    ; break;
			case portC:DDRC_reg|= (1<<pin)    ; break;
			case portD:DDRD_reg|= (1<<pin)   ; break;
			}



}

	else if(direction==input){
	switch(port){

			case portA:DDRA_reg&= ~(1<<pin); break;
			case portB:DDRB_reg&= ~(1<<pin); break;
			case portC:DDRC_reg&= ~(1<<pin)	; break;
			case portD:DDRD_reg&= ~(1<<pin); break;
				}



 }

}
}
void set_PIN_value(port_ID port,pin_ID pin,port_VALUE direction){
	if(port <=portD  && pin<=7){
		if(direction==high){
		switch(port){

				case portA:PORTA_reg|= (1<<pin); break;
				case portB:PORTB_reg|= (1<<pin)    ; break;
				case portC:PORTC_reg|= (1<<pin)    ; break;
				case portD:PORTD_reg|= (1<<pin)   ; break;
				}



	}

		else	if(direction==input){
		switch(port){

				case portA:PORTA_reg&= ~(1<<pin); break;
				case portB:PORTB_reg&= ~(1<<pin); break;
				case portC:PORTC_reg&= ~(1<<pin)	; break;
				case portD:PORTD_reg&= ~(1<<pin); break;
					}



	 }

	}
}
port_VALUE get_pin_value(port_ID port,pin_ID pin){

volatile port_VALUE pinval;
if(port<=portD&&pin<=7){
	switch(port){

	case portA:pinval=get(PINA_reg,pin)  ;break;
	case portB:pinval=get(PINB_reg,pin)  ;break;
	case portC:pinval=get(PINC_reg,pin)  ;break;
	case portD:pinval=get(PIND_reg,pin);break;






	}
}

return pinval;
}




