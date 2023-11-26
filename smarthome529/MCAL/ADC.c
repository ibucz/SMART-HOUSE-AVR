/*
 * ADC.c
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */


#include<avr/io.h>
#include<util/delay.h>
void ADC_init(void)
{
	DDRA=0x0;			    /* Make ADC port as input */
	ADCSRA = 0x87;			/* Enable ADC, fr/128  */
	ADMUX = 0x40;			/* Vref: Avcc, ADC channel: 0 */
}

unsigned int read_adc(char channel)
{
	int Ain,AinLow;

	ADMUX=ADMUX|(channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while((ADCSRA&(1<<ADIF))==0);	/* Monitor end of conversion interrupt */

	_delay_us(10);
	AinLow = (int)ADCL;		     /* Read lower byte*/
	Ain = (int)(ADCH<<8);		/* Read higher 2 bits and
					Multiply with weight */
	Ain = Ain + AinLow;
	return(Ain);			/* Return digital value*/
}





