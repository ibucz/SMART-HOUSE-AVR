/*
 * DIO.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef DIO_H_
#define DIO_H_
#include"DIO_dir.h"
#include"DIO_plat_forms.h"
#include"DIO_bitmath.h"

void set_port_direction(port_ID port,port_DIR direction);
void set_port_value(port_ID port,port_VALUE direction);
void set_PIN_direction(port_ID port,pin_ID pin,port_DIR direction);
void set_PIN_value(port_ID port,pin_ID pin,port_VALUE direction);
port_VALUE get_pin_value(port_ID port,pin_ID pin);
#endif /* DIO_H_ */
