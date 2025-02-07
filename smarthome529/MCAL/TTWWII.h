/*
 * TTWWII.h
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */

#ifndef TTWWII_H_
#define TTWWII_H_
#define slave_mode 1    // 0 is slave mode is send data from slave -  1 is data to slave
#include<util/delay.h>
#include<avr/io.h>
#include <stdlib.h>

void i2c_init();//common


//slave
void i2c_slave_address(unsigned char address);
void i2c_write(unsigned char data);  //slave
unsigned char i2c_read(unsigned char ack);


  // master
unsigned char data_fromSlave(unsigned char slaveADDRESS_lastBITis1);
void data_toSLAVE(unsigned char slave_addressLASTBIT0,unsigned char dataTOslave);
void i2c_start(void);
void i2c_stop(void);

//A0

void write_eeprom(unsigned char address_eepromDEVICEandLASTbitFORwrite,unsigned char locationADDRESS_eeprom,unsigned char data_onEEPROM);
unsigned char read_fromEEPROM(unsigned char*data,unsigned char addressDEVICEeeprom,unsigned char locationADDRESSeeprom);
#endif /* TTWWII_H_ */
