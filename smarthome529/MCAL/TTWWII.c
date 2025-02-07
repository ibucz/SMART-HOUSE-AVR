/*
 * TTWWII.c
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */
#include"TTWWII.h"

void i2c_write(unsigned char data)
{
//wrong: "clear twint first and then access data register" because when acess data within ,twint is clear, collision hapeen and twwr is set
TWDR=data;                    //note:acess data register first because avoiding collision
TWCR=(1<<TWEN)|(1<<TWINT);    //note: clear twint by put 1
while((TWCR&(1<<TWINT))==0);  // when 0 not finished when 1 release
}
//***************************************************************
void i2c_start(void)
{
	TWCR =(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //clear for future job,start condtion,enable twi
	while((TWCR&(1<<TWINT))==0);  // when 0 not finished when 1 release(software delay>>streches the clk)
}
//*******************************************************************

void i2c_stop(void)
{
	TWCR =(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);   // clear for future job,stop condtion,enable twi

}
//*************************************************************
unsigned char i2c_read(unsigned char ack)
{
	if(ack==0)  //this when set ack and uses in read multi byte with repeated start

	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); //set enable ack

	else// note this function read one byte only
		TWCR=(1<<TWEN)|(1<<TWINT);

	while((TWCR&(1<<TWINT))==0);
	// when 0 not finished when 1 release(software delay>>streches the clk)
	// note: when read waiting first and then return data
	//polling here for waiting slave sent data in TWDR
	return TWDR;
}
//***************************************************************
void i2c_init()
{
	TWSR=0x00;   //no prescaler for two last bits         TWPS=0
	TWBR=2;   // to set the speed 400kb/s
	TWCR=(1<<TWEN)|(1<<TWEA); // enable the TWI module and acknowledge

}
//******************************************************************
void i2c_slave_address(unsigned char address)
{    // if mc becomes slave   (note that mc here is master and rarely used this function)
	TWAR=address;  //adress is 7 bit and last bit for twi respond to general call if this bit set by 1
	// for general call address 0x01(all slaves receieve the master's data)
}
//********************************************************************
unsigned char data_fromSlave(unsigned char slaveADDRESS_lastBITis1)
{
	 unsigned char rece_data=0;    //  var that put in it data from slave microcontroller 2

	    i2c_start();
		i2c_write(slaveADDRESS_lastBITis1); //transmit slave address and last bit for read 1
        rece_data=i2c_read(1);  //1 is non ack that read only one byte
		i2c_stop();
        _delay_ms(50);
        return rece_data;


}
//***********************************************************************
void data_toSLAVE(unsigned char slave_addressLASTBIT0,unsigned char dataTOslave)
{

    i2c_start();
    i2c_write(slave_addressLASTBIT0);
    i2c_write(dataTOslave);
    i2c_stop();
}
//***********************************************************************
void write_eeprom(unsigned char address_eepromDEVICEandLASTbitFORwrite,unsigned char locationADDRESS_eeprom,unsigned char data_onEEPROM)
 {
	 //write
	 i2c_start();
	 i2c_write(address_eepromDEVICEandLASTbitFORwrite); //eeprom device address      0xA0
	 i2c_write(locationADDRESS_eeprom);//eeprom address register     0X01

//	 i2c_start();
//	 i2c_write(address_eepromDEVICEandLASTbitFORwrite);
	 i2c_write(data_onEEPROM);  // data
	 i2c_stop();
	 _delay_ms(50);
 }
unsigned char read_fromEEPROM(unsigned char* data,unsigned char addressDEVICEeeprom,unsigned char locationADDRESSeeprom)
{
	//unsigned char rece_data_eeprom=0;
	//read
	//*************
	i2c_start();
	i2c_write(addressDEVICEeeprom);  //last bit is to zero for write
	i2c_write(locationADDRESSeeprom);

	i2c_start();  //repeated start when read : no other master can pull line
	//within read no other master can pull line
	i2c_write(addressDEVICEeeprom|1);  //last bit is to 1 for read
	*data=i2c_read(1);  //1 is non ack that read only one byte
	i2c_stop();
	return *data;
}
