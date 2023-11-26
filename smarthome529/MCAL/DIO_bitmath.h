/*
 * DIO_bitmath.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef MCAL_DIO_BITMATH_H_
#define MCAL_DIO_BITMATH_H_
#define SET_BIT(Register,Pin)       (Register |=(1<<Pin))
#define CLEAR_BIT(Register,Pin)     (Register &=~(1<<Pin))
#define BIT_SET(Register,Pin)       (Register &(1<<Pin))
#define BIT_CLEARED(Register,Pin)   (!(Register &(1<<Pin)))
#define get(port,pin)     ((port>>pin)&0x01)
#define toggle(port,pin) (port^=(1<<pin))
#endif /* MCAL_DIO_BITMATH_H_ */
