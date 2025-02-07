/*
 * Keypad.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef HALL_KEYPAD_H_
#define HALL_KEYPAD_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../MCAL/PLATFORM_TYPES.h"
#include "../MCAL/DIO.h"
#include"../MCAL/DIO_bitmath.h"

/*Wiring diagram*/

/* * * *c1  c2 c3 c4
 * row1  1  2  3  A
 * row2  4  5  6  B
 * row3  7  8  9  C
 * row4  *  0  #  D
 */
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN  PINA
#define KEYPAD_PORT_DIR DDRA
#define KEYPAD_PORT_OUT2 PORTC
#define KEYPAD_PORT_DIR2 DDRC
#define FIRST_ROW  (0)
#define SECOND_ROW (1)
#define THIRD_ROW  (2)
#define FOURTH_ROW (3)
#define FIRST_COL  (4)
#define SECOND_COL (5)
#define THIRD_COL  (6)
#define FOURTH_COL (7)
void  Keypad_init(void);
uint8 Keypad_get_pressed_key(void);
#endif /* HALL_KEYPAD_H_ */
