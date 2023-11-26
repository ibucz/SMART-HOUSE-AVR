/*
 * DIO_dir.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */


#ifndef MCAL_DIO_DIR_H_
#define MCAL_DIO_DIR_H_

typedef enum{
	portA,
	portB,
	portC,
	portD
}port_ID;
typedef enum{
	pin0,
	pin1,
	pin2,
	pin3,
	pin4,
	pin5,
	pin6,
	pin7,

}pin_ID;
typedef enum{


	input,
	output
}port_DIR;
typedef enum{


	low,
	high
}port_VALUE;



#endif /* MCAL_DIO_DIR_H_ */
