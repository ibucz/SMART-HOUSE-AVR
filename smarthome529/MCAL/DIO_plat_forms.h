/*
 * DIO_plat_forms.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef MCAL_DIO_PLAT_FORMS_H_
#define MCAL_DIO_PLAT_FORMS_H_

#define PORTA_reg     (*(volatile unsigned char*)0x3B)
#define DDRA_reg     (*(volatile unsigned char*)0x3A)
#define PINA_reg     (*(volatile unsigned char*)0x39)

#define PORTB_reg     (*(volatile unsigned char*)0x38)
#define DDRB_reg     (*(volatile unsigned char*)0x37)
#define PINB_reg    (*(volatile unsigned char*)0x36)


#define PORTC_reg     (*(volatile unsigned char*)0x35)
#define DDRC_reg      (*(volatile unsigned char*)0x34)
#define PINC_reg      (*(volatile unsigned char*)0x33)



#define PORTD_reg      (*(volatile unsigned char*)0x32)
#define DDRD_reg     (*(volatile unsigned char*)0x31)
#define PIND_reg      (*(volatile unsigned char*)0x30)

#endif /* MCAL_DIO_PLAT_FORMS_H_ */
