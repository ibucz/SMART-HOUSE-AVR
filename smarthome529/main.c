/*
 * smarthome529.c
 *
 * Created: 11/06/2023 
 * Author : AZIZ
 */ 

#include "MCAL/ADC.h"
#include"HALL/LCD.h"
#include"HALL/Keypad.h"
#include"MCAL/TTWWII.h"
#include"MCAL/uart.h"
#include "MCAL/PLATFORM_TYPES.h"
#include<stdlib.h>
#define key_not_pressed  0
#define user_mode '2'
#define admin_mode '1'
#define max_name_length  4
#define pass_length 4
//Admin Addresses
#define name_address_ad (unsigned char)0x02
#define pass_address_ad (unsigned char)0x06
//Users Addresses
#define pass_address_u1 (unsigned char)0x0E
#define pass_address_u2 (unsigned char)0x16
#define pass_address_u3 (unsigned char)0x1E
#define name_address_u1 (unsigned char)0x0A
#define name_address_u2 (unsigned char)0x12
#define name_address_u3 (unsigned char)0x1A
#define users_address_add (unsigned char)0x00
#define remove_sign (unsigned char)0xFF
#define led_status_ad (unsigned char)0x3F
#define led_status_u (unsigned char)0x41
#define max_trials (3)

unsigned char trial=0;
unsigned char ziz;
unsigned char i=0;
unsigned char motor_steps[2] = {96, 48};
unsigned char key=key_not_pressed;
unsigned char admin_pass[pass_length]="1812";
unsigned char admin_name[max_name_length]="saso";
unsigned char user1_name[max_name_length];
unsigned char user2_name[max_name_length];
unsigned char user3_name[max_name_length];
unsigned char user1_pass[pass_length];
unsigned char user2_pass[pass_length];
unsigned char user3_pass[pass_length];
unsigned char user_mode_name[max_name_length];
unsigned char user_mode_pass[pass_length];
unsigned char recieve_name[max_name_length];
unsigned char recieve_pass[pass_length];
unsigned char name_length_count=0;
unsigned char check_admin_name[max_name_length];
unsigned char check_admin_pass[pass_length];
unsigned char choice;
unsigned char total_users='0';
unsigned char digit_1=0;
unsigned char digit_2=0;
unsigned char count_users=0;
unsigned char admin_state=0;
unsigned char user_state=0;
unsigned char user_number='0';
unsigned char data_admin_led=0;
unsigned char data_user_led=0;
double adcA=0.0;
double temperature=0.0;
char tempSTR[10];
int brightness=0;

void menu_admin();
void menu_user();
void add_user();
void remove_user();
void remove_user_menu();
void user_menu_data();
void names();
void write_eeprom_pass_admin(unsigned char* pass);
void write_eeprom_name_admin( unsigned char* name);
void admin_init();
void recieve_string(unsigned char* rec_data);
void check(unsigned char* rec_name,unsigned char* rec_pass,unsigned char* checker,unsigned char name_add,unsigned char pass_add,unsigned char eeprom_device_address);
void gate_control();
int array_compare(unsigned char arr1[],unsigned char arr2[]);
void control_leds_admin(unsigned char choose_led);
void control_leds_user(unsigned char led_choose);
void read_data(unsigned char* id,unsigned char* pass,unsigned char id_add,unsigned char pass_add);
void user_cases();
void init_fast_pwm();
void duty_cycle_update(unsigned int duty_cycle);
void AC_Control();
void Dimmer_Control();
void buzzer();
void PWM_init();
void PWM_set_duty_cycle(uint16_t duty_cycle);
 int main(){
	LCD_Init();
	Keypad_init();
    uart_init_rx(9600);
    uart_init_tx(9600);
    ADC_init();
    i2c_init();
    LCD_Init();
    admin_init();

    while(1){
    	names();
    	uart_recie(&key);
        uart_transmit_string("\r\n");
switch(key){
case admin_mode:
LCD_Send_String_Row_Column(0,0,"admin_mode");
check(recieve_name,recieve_pass,&admin_state,name_address_ad,pass_address_ad,0xA0);
while(admin_state==1)
{
	menu_admin();
	uart_recie(&choice);
	uart_transmit_string("\r\n");
	if(choice=='7')
	{
		key=key_not_pressed;
		choice='0';
		break;
	}
switch (choice){
	case '1':
	{read_fromEEPROM(&ziz,0xA0,users_address_add);
	total_users=ziz;
	write_eeprom(0xA0,users_address_add,total_users);
	read_fromEEPROM(&count_users,0xA0,users_address_add);
	add_user();
		break;
	}

	case '2':
	{	read_fromEEPROM(&ziz,0xA0,users_address_add);
	total_users=ziz;
	write_eeprom(0xA0,users_address_add,total_users);
			read_data(user1_name,user1_pass,name_address_u1,pass_address_u1);
		read_data(user2_name,user2_pass,name_address_u2,pass_address_u2);
		read_data(user3_name,user3_pass,name_address_u3,pass_address_u3);
		//remove user
		read_fromEEPROM(&ziz,0xA0,users_address_add);
		total_users=ziz;
		remove_user();

		break;
	}
	case '3':
	{	LCD_Send_String_Row_Column(0,0,"admin_mode");
	LCD_Send_String_Row_Column(1,0,"door_open");
		gate_control();
		LCD_Clear_Display();
		break;
	}
	case '4':


		read_fromEEPROM(&data_admin_led,0xA0,led_status_ad);
		read_fromEEPROM(&data_user_led,0xA0,led_status_u);

	{ if(admin_state==1){
		control_leds_admin(data_admin_led);
		uart_transmit_string("enter led 1~5\r\n");
		uart_recie(&key);
		control_leds_admin(key);
	}
	else{
		control_leds_user(data_user_led);
		LCD_Send_String_Row_Column(0,0,"enter led 1~5\r\n");
		do{
			key=Keypad_get_pressed_key();
		}while(key==key_not_pressed);
		control_leds_user(key);
	}
		
		key=key_not_pressed;
		//control lamps
		break;
	}
	case '5':
	{ 
		AC_Control();
		//control AC
		break;
	}
	case '6':
	{
		PWM_init();
		Dimmer_Control();
		break;
	}
	default:
		choice='0';
		uart_transmit_string("Please Enter Right Choice\r\n");
		break;
}

}
break;
case user_mode:
	{
	read_fromEEPROM(&ziz,0xA0,users_address_add);
total_users=ziz;
write_eeprom(0xA0,users_address_add,total_users);
		read_data(user1_name,user1_pass,name_address_u1,pass_address_u1);
	read_data(user2_name,user2_pass,name_address_u2,pass_address_u2);
	read_data(user3_name,user3_pass,name_address_u3,pass_address_u3);
	user_cases();

		user_menu_data();
	}
}
}
}

void check(unsigned char* rec_name,unsigned char* rec_pass,unsigned char* checker,unsigned char name_add,unsigned char pass_add, unsigned char eeprom_device_address){
unsigned char data=0;
unsigned char digit=0;
unsigned char address=0;
unsigned char address_n=0;
unsigned char address_p=0;
unsigned char exist_name=0;
unsigned char exist_pass=0;

address_n=name_add;
while(trial<3){
	uart_transmit_string("enter_name\r\n");
	recieve_string(recieve_name);
	_delay_ms(300);
	uart_transmit_string("--------------\r\n");
	_delay_ms(300);
	uart_transmit_string("enter_pin_four_digit\r\n");
	recieve_string(recieve_pass);
	_delay_ms(300);
	uart_transmit_string("--------------\r\n");
	_delay_ms(300);
while(digit<4){

    address=digit;
    read_fromEEPROM(&data,eeprom_device_address,(address+address_n));
    if(data==rec_name[digit]){
        exist_name++;
    }
    digit++;
}
digit=0;
address=0;
address_p=pass_add;
while(digit<4){

    address=digit;
read_fromEEPROM(&data,eeprom_device_address,(address+(address_p)));
    if(data==rec_pass[digit]){
        exist_pass++;
    }
    digit++;
}
if(exist_name==4&&exist_pass==4){
    uart_transmit_string("Successful\r\n");
    *checker=1;
    _delay_ms(1000);
    exist_name=0;
    exist_pass=0;
    digit=0;
    address=0;
    trial=0;
    break;
}
 if(exist_name!=4&&exist_pass==4){
    uart_transmit_string("Invalid_Name\r\n");
   uart_transmit_string("Please Enter Right Information\r\n");
    *checker=0;
    exist_name=0;
      exist_pass=0;
      digit=0;
      address=0;
      trial++;
    _delay_ms(500);
    continue;
}
if(exist_name==4&&exist_pass!=4){
    uart_transmit_string("Invalid_Pass\r\n");
    uart_transmit_string("Please Enter Right Information\r\n");
    *checker=0;
    exist_name=0;
      exist_pass=0;
      digit=0;
      address=0;
      trial++;
    _delay_ms(500);
    continue;
}
if(exist_name!=4&&exist_pass!=4){
    uart_transmit_string("invlaid_name_and_pass\r\n");
    uart_transmit_string("Please Enter Right Information\r\n");
    *checker=0;
    exist_name=0;
      exist_pass=0;
      digit=0;
      address=0;
      trial++;
    _delay_ms(500);
    continue;
}
}
if(trial>2){
	uart_transmit_string("wait_5sec_for_buzzer\r\n");
	buzzer();
	trial=0;
}
}
void menu_admin()
{
	uart_transmit_string("**********\r\n");
	uart_transmit_string("1-Add user\r\n");
	uart_transmit_string("2-Remove user\r\n");
	uart_transmit_string("3-Door Control\r\n");
	uart_transmit_string("4-Lamp Control\r\n");
	uart_transmit_string("5-AC Control\r\n");
	uart_transmit_string("6-Dimming Control\r\n");
	uart_transmit_string("7-Return Home\r\n");
}
void menu_user()
{
	LCD_Send_String_Row_Column(0,0,"1-leds");
	LCD_Send_String_Row_Column(0,8,"2-AC");
	LCD_Send_String_Row_Column(1,0,"3-Dimmer");
	LCD_Send_String_Row_Column(1,9,"4-home");
}

void gate_control()
{
	DDRB=0xFF;
	LCD_Clear_Display();
	LCD_Send_String_Row_Column(0,0,"Door Opening");
	uart_transmit_string("Door Opening -Wait 5 Seconds\r\n");
	PORTB = (PORTB & 0x0F) | (motor_steps[1] & 0b11111111); // 90
	 _delay_ms(5000);
	 PORTB = (PORTB & 0x0F) | (motor_steps[0] & 0b11111111); // 0

}
void add_user()
{

	if(count_users=='0'){

		uart_transmit_string("enter_id_for_user1\r\n");

		recieve_string(user1_name);
		while(digit_1<4){
			write_eeprom(0xA0,name_address_u1+digit_1,user1_name[digit_1]);
			digit_1++;
		}
		digit_1=0;
		uart_transmit_string("--------------\r\n");
		_delay_ms(300);

		uart_transmit_string("enter_pin_four_digit_for_user1\r\n");
		recieve_string(user1_pass);
		while(digit_1<4){
			write_eeprom(0xA0,pass_address_u1+digit_1,user1_pass[digit_1]);
			digit_1++;
		}
		digit_1=0;
		total_users='1';
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("--------------\r\n");
	}

	else if(count_users=='1'){
		uart_transmit_string("enter_id_for_user2\r\n");

		recieve_string(user2_name);
		while(digit_1<4){
			write_eeprom(0xA0,name_address_u2+digit_1,user2_name[digit_1]);
			digit_1++;
		}
		digit_1=0;
		uart_transmit_string("--------------\r\n");
		_delay_ms(300);

		uart_transmit_string("enter_pin_four_digit_for_user2\r\n");
		recieve_string(user2_pass);
		while(digit_1<4){
			write_eeprom(0xA0,pass_address_u2+digit_1,user2_pass[digit_1]);
			digit_1++;
		}
		digit_1=0;
		total_users='2';
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("--------------\r\n");
	}
	else if(count_users=='2'){

		uart_transmit_string("enter_id_for_user3\r\n");

		recieve_string(user3_name);
		while(digit_1<4){
			write_eeprom(0xA0,name_address_u3+digit_1,user3_name[digit_1]);
			digit_1++;
		}
		digit_1=0;
		uart_transmit_string("--------------\r\n");
		_delay_ms(300);

		uart_transmit_string("enter_pin_four_digit_for_user3\r\n");
		recieve_string(user3_pass);
		while(digit_1<4){
			write_eeprom(0xA0,pass_address_u3+digit_1,user3_pass[digit_1]);
			digit_1++;
		}
		digit_1=0;
		total_users='3';
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("--------------\r\n");
	}

	else
		{
			uart_transmit_string("There is Max Number of users\r\n");
			uart_transmit_string("--------------\r\n");
			admin_state=0;
		}
}

void names()
{
	 uart_transmit_string("1-admin_mode\r\n");
	 uart_transmit_string("2-user_mode\r\n");

}
void write_eeprom_pass_admin(unsigned char* pass){
 	  unsigned char i;
 	 for(i=0;i<pass_length;i++)
 	 {
 		 write_eeprom(0XA0,pass_address_ad+i,pass[i]);
 	 }
  }
void write_eeprom_name_admin( unsigned char* name){
	unsigned char j;
	 for(j=0;name[j]!='\0';j++){
		 write_eeprom(0XA0,name_address_ad+j,name[j]);
	 }
}
void admin_init(){
 	 write_eeprom_name_admin(admin_name);
 	 write_eeprom_pass_admin(admin_pass);

}
void recieve_string(unsigned char* rec_data){
	 int i=0;
		unsigned char recieve_digit=key_not_pressed;
	 for(i=0;(i<max_name_length);i++){
	 	uart_recie(&recieve_digit);
	 	if(recieve_digit==0x0D){
	 		 		break;
	 		 	}
	 	name_length_count++;
	 	rec_data[i]=recieve_digit;
	 	recieve_digit=key_not_pressed;
	 }
}
void remove_user()
{
	unsigned char user_check_name[max_name_length];
	unsigned char user_check_pass[pass_length];
	if(total_users=='0')
	{
		uart_transmit_string("No users to Remove\r\n");
		write_eeprom(0xA0,users_address_add,total_users);
	}
	else
	{
		uart_transmit_string("enter_id_for_user\r\n");
		recieve_string(user_check_name);
		uart_transmit_string("---------------\r\n");
		uart_transmit_string("enter_pin_for_user\r\n");
		recieve_string(user_check_pass);
		uart_transmit_string("---------------\r\n");
	if(array_compare(user1_name,user_check_name)==1 && array_compare(user1_pass,user_check_pass)==1)
			{
				while (digit_2 < 4) {
					write_eeprom(0xA0, (name_address_u1 + digit_2),
							remove_sign);
					write_eeprom(0xA0, (pass_address_u1 + digit_2),
							remove_sign);
					user1_name[digit_2]=0;
					user1_pass[digit_2]=0;
					digit_2++;
				}
				remove_user_menu();

			}
	else if(array_compare(user_check_name,user2_name)==1 && array_compare(user_check_pass,user2_pass)==1)
			{

				while (digit_2 < 4) {
					write_eeprom(0xA0, (name_address_u2 + digit_2),
							remove_sign);
					write_eeprom(0xA0, (pass_address_u2 + digit_2),
							remove_sign);
					user2_name[digit_2]=0;
					user2_pass[digit_2]=0;
					digit_2++;
				}
				remove_user_menu();

			}
else if(array_compare(user_check_name,user3_name)==1 && array_compare(user_check_pass,user3_pass)==1)
			{
					while (digit_2 < 4) {
						write_eeprom(0xA0, (name_address_u3 + digit_2),
								remove_sign);
						write_eeprom(0xA0, (pass_address_u3 + digit_2),
								remove_sign);
						user3_name[digit_2]=0;
						user3_pass[digit_2]=0;
						digit_2++;
					}
						remove_user_menu();
			}
else
			{
				uart_transmit_string("User not Found\r\n");
			}
	}
}
int array_compare(unsigned char arr1[],unsigned char arr2[])
{
	int i=0;
	int checker=0;
	for(i=0;i<max_name_length;i++)
	{
		if(arr1[i]==arr2[i])
		{
			checker++;
		}
	}
	if(checker==max_name_length)
	{
		return 1;
	}
	return 0;
}
void remove_user_menu()
{

	user_number--;
	digit_2 = 0;
	uart_transmit_string("User Found and Deleted\r\n");
	total_users--;
	if(total_users=='0')
	{
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("Total Users equals 0\r\n");
	}
	if(total_users=='1')
	{
		total_users='1';
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("Total Users equals 1\r\n");
	}
	else if(total_users=='2')
	{
		total_users='2';
		write_eeprom(0xA0,users_address_add,total_users);
		uart_transmit_string("Total Users equals 2\r\n");
	}
	else
	{}
}
void user_menu_data()

{admin_state=0;
	int counter=0;
	unsigned char position=0;
	while(counter<3)
	{key=key_not_pressed;
	LCD_Clear_Display();
		LCD_Send_String_Row_Column(0,0,"enter_name");
		for(position=0;position<4;position++){
			key=key_not_pressed;
			do{
				key=Keypad_get_pressed_key();
				_delay_ms(300);
				user_mode_name[position]=key;
				LCD_GoTO_Row_Column(1,position);
				LCD_Send_character(key);

			}while(key==key_not_pressed);
		}
		_delay_ms(1000);
		LCD_Clear_Display();
		key=key_not_pressed;
	_delay_ms(300);
	LCD_Send_String_Row_Column(0,0,"enter_pin_4_num");
	for(position=0;position<4;position++){
			key=key_not_pressed;
			do{
				key=Keypad_get_pressed_key();
				_delay_ms(200);
				user_mode_pass[position]=key;
				LCD_GoTO_Row_Column(1,position);
				LCD_Send_character(key);
			}while(key==key_not_pressed);
		}
	_delay_ms(1000);
			LCD_Clear_Display();
			key=key_not_pressed;

		if(array_compare(user_mode_name,user1_name)==1 && array_compare(user_mode_pass,user1_pass))
		{LCD_Send_String_Row_Column(0,0,"Welcome_Home");

		user_state=1;
		_delay_ms(1000);
						while(user_state){
						LCD_Clear_Display();
						menu_user();
							do{
								key=Keypad_get_pressed_key();
								_delay_ms(200);
							}while(key==key_not_pressed);
							_delay_ms(1000);
							LCD_Clear_Display();
							switch (key){
							case '1':

							{admin_state=0;
								read_fromEEPROM(&data_admin_led,0xA0,led_status_ad);
							read_fromEEPROM(&data_user_led,0xA0,led_status_u);

						 if(admin_state==1){
							control_leds_admin(data_admin_led);
							uart_transmit_string("enter led 1~5");
							uart_recie(&key);
							control_leds_admin(key);
						}
						else{
							control_leds_user(data_user_led);
							LCD_Send_String_Row_Column(0,0,"enter led 1~5");
							do{
								key=Keypad_get_pressed_key();
							}while(key==key_not_pressed);
							control_leds_user(key);
						}
							key=key_not_pressed;
							user_state=1;
								break;
							}
							case '2':
							{ 
								AC_Control();
								key=key_not_pressed;
								break;
							}
							case '3':
							{
								Dimmer_Control();
								key=key_not_pressed;
								break;
							}
							case '4':
							{
								user_state=0;
								key=key_not_pressed;
								break;
							}
							default:
							{
								key=key_not_pressed;
								break;
							}

							}
						}
						break;
						}

		else if(array_compare(user_mode_name,user2_name)==1 && array_compare(user_mode_pass,user2_pass))
		{
			LCD_Send_String_Row_Column(0,0,"Welcome_Home");
			user_state=1;
			_delay_ms(1000);
							while(user_state){
							LCD_Clear_Display();
							menu_user();
								do{
									key=Keypad_get_pressed_key();
									_delay_ms(200);
								}while(key==key_not_pressed);
								_delay_ms(1000);
								LCD_Clear_Display();
								switch (key){
								case '1':

								{admin_state=0;
									read_fromEEPROM(&data_admin_led,0xA0,led_status_ad);
								read_fromEEPROM(&data_user_led,0xA0,led_status_u);

							 if(admin_state==1){
								control_leds_admin(data_admin_led);
								uart_transmit_string("enter led 1~5");
								uart_recie(&key);
								control_leds_admin(key);
							}
							else{
								control_leds_user(data_user_led);
								LCD_Send_String_Row_Column(0,0,"enter led 1~5");
								do{
									key=Keypad_get_pressed_key();
								}while(key==key_not_pressed);
								control_leds_user(key);
							}

								key=key_not_pressed;
								user_state=1;
									break;
								}
								case '2':
								{ 
									AC_Control();
									key=key_not_pressed;
									break;
								}
								case '3':
								{
									Dimmer_Control();
									key=key_not_pressed;
									break;
								}
								case '4':
								{
									user_state=0;
									key=key_not_pressed;
									break;
								}
								default:
								{
									key=key_not_pressed;
									break;
								}

								}
							}
							break;
							}
		else if(array_compare(user_mode_name,user3_name)==1 && array_compare(user_mode_pass,user3_pass))
		{
			LCD_Send_String_Row_Column(0,0,"Welcome_Home");
		
			user_state=1;
			_delay_ms(1000);
						 while(user_state){
							LCD_Clear_Display();
							menu_user();
								do{
									key=Keypad_get_pressed_key();
									_delay_ms(200);
									
								}while(key==key_not_pressed);
								_delay_ms(1000);
								LCD_Clear_Display();
						    
								switch (key){
								case '1':

								{admin_state=0;
									read_fromEEPROM(&data_admin_led,0xA0,led_status_ad);
								read_fromEEPROM(&data_user_led,0xA0,led_status_u);

							 if(admin_state==1){
								control_leds_admin(data_admin_led);
								uart_transmit_string("enter led 1~5");
								uart_recie(&key);
								control_leds_admin(key);
							}
							else{
								control_leds_user(data_user_led);
								LCD_Send_String_Row_Column(0,0,"enter led 1~5");
								do{
									key=Keypad_get_pressed_key();
								}while(key==key_not_pressed);
								control_leds_user(key);
							}
								//control_leds_admin(key);
								key=key_not_pressed;
								user_state=1;
									//key=key_not_pressed;
									break;
								}
								case '2':
								{ //init_fast_pwm();
									AC_Control();
									key=key_not_pressed;
									break;
								}
								case '3':
								{
									Dimmer_Control();
									key=key_not_pressed;
									break;
								}
								case '4':
								{
									user_state=0;
									key=key_not_pressed;
									break;
								}
								default:
								{
									key=key_not_pressed;
									break;
								}

								}
							}
							break;
							}
		else
		{counter++;
			_delay_ms(500);
		LCD_Clear_Display();
		LCD_Send_String_Row_Column(0,0,"user_not_found");
		_delay_ms(500);
		}
	}
	if(counter==3)

	{
		_delay_ms(500);
				LCD_Clear_Display();
				LCD_Send_String_Row_Column(0,0,"system_off");
				buzzer();
				LCD_Clear_Display();
	}
}
void control_leds_admin(unsigned char led_choose){
	unsigned char data=0;
	DDRC|=(1<<PC2)|(1<<PC3);
	DDRD|=(1<<PD2)|(1<<PD3);
DDRA|=(1<<PA7);
_delay_ms(100);
LCD_Clear_Display();
write_eeprom(0xA0,led_status_ad,led_choose);
read_fromEEPROM(&data,0xA0,led_status_ad);
	switch(data){
	case'1':
		PORTC|=(1<<PC2);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
		CLEAR_BIT(PORTD,PD3);
		CLEAR_BIT(PORTD,PD2);
		CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_1_on");
		break;
	case'2':
		PORTC|=(1<<PC3);
		CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTD,PD3);
			CLEAR_BIT(PORTD,PD2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_2_on");
		break;
	case '3':
		PORTD|=(1<<PD2);
		CLEAR_BIT(DDRC,PC3);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(PORTD,PD3);
			CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_3_on");
	break;

	case'4':
		PORTD|=(1<<PD3);
		CLEAR_BIT(DDRC,PC3);
			CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTD,PD2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_4_on");
	break;
	case '5':
		PORTA|=(1<<PA7);
		CLEAR_BIT(DDRC,PC2);
					CLEAR_BIT(DDRC,PC3);
					CLEAR_BIT(DDRD,PD2);
					CLEAR_BIT(DDRD,PD3);
					CLEAR_BIT(DDRC,PC2);
				CLEAR_BIT(PORTC,PC3);
					CLEAR_BIT(PORTC,PC2);
					CLEAR_BIT(PORTD,PD2);
					CLEAR_BIT(PORTD,PD3);
					CLEAR_BIT(PORTC,PC3);
		_delay_ms(100);

		LCD_Send_String_Row_Column(0,0,"led_5_on");
	break;


	}



}
void control_leds_user(unsigned char led_choose){
	unsigned char data=0;
	DDRC|=(1<<PC2)|(1<<PC3);
	DDRD|=(1<<PD2)|(1<<PD3);
DDRA|=(1<<PA7);
_delay_ms(100);
LCD_Clear_Display();
write_eeprom(0xA0,led_status_u,led_choose);
read_fromEEPROM(&data,0xA0,led_status_u);
	switch(data){
	case'1':
		PORTC|=(1<<PC2);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
		CLEAR_BIT(PORTD,PD3);
		CLEAR_BIT(PORTD,PD2);
		CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_1_on");
		break;
	case'2':
		PORTC|=(1<<PC3);
		CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTD,PD3);
			CLEAR_BIT(PORTD,PD2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_2_on");
		break;
	case '3':
		PORTD|=(1<<PD2);
		CLEAR_BIT(DDRC,PC3);
			CLEAR_BIT(DDRD,PD3);
			CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(PORTD,PD3);
			CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_3_on");
	break;

	case'4':
		PORTD|=(1<<PD3);
		CLEAR_BIT(DDRC,PC3);
			CLEAR_BIT(DDRC,PC2);
			CLEAR_BIT(DDRD,PD2);
			CLEAR_BIT(DDRA,PA7);
		CLEAR_BIT(PORTC,PC3);
			CLEAR_BIT(PORTC,PC2);
			CLEAR_BIT(PORTD,PD2);
			CLEAR_BIT(PORTA,PA7);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_4_on");
	break;
	case '5':
		PORTA|=(1<<PA7);
		CLEAR_BIT(DDRC,PC2);
					CLEAR_BIT(DDRC,PC3);
					CLEAR_BIT(DDRD,PD2);
					CLEAR_BIT(DDRD,PD3);
					CLEAR_BIT(DDRC,PC2);
				CLEAR_BIT(PORTC,PC3);
					CLEAR_BIT(PORTC,PC2);
					CLEAR_BIT(PORTD,PD2);
					CLEAR_BIT(PORTD,PD3);
					CLEAR_BIT(PORTC,PC3);
		_delay_ms(100);
		LCD_Send_String_Row_Column(0,0,"led_5_on");
	break;
	}
}
void read_data(unsigned char* id,unsigned char* pass,unsigned char id_add,unsigned char pass_add){
	unsigned char pos=0;
	unsigned char recent=0;
	while(pos<4){
		read_fromEEPROM(&recent,0xA0,id_add+pos);
		id[pos]=recent;
		pos++;
		recent=0;
	}
	pos=0;
	while(pos<4){
		read_fromEEPROM(&recent,0xA0,id_add+pos);
		pass[pos]=recent;
		pos++;
		recent=0;
	}
	pos=0;
}
void user_cases(){

	for(i=0;i<4;i++){
					if((user1_name[i]!=0xff&&user1_pass[i]!=0xff) &&(user2_name[i]!=0xff&&user2_pass[i]!=0xff) && (user3_name[i]!=0xff&&user3_pass[i]!=0xff)){
						total_users='3';
					}//case 1
					if((user1_name[i]==0xff&&user1_pass[i]==0xff) && (user2_name[i]!=0xff&&user2_pass[i]!=0xff) && (user3_name[i]!=0xff&&user3_pass[i]!=0xff)){
										total_users='2';
									}//case 2

					if((user1_name[i]!=0xff&&user1_pass[i]!=0xff) && (user2_name[i]==0xff&&user2_pass[i]==0xff) && (user3_name[i]!=0xff&&user3_pass[i]!=0xff)){
										total_users='2';
									}//case 3
					if((user1_name[i]==0xff&&user1_pass[i]==0xff) &&(user2_name[i]==0xff&&user2_pass[i]==0xff) && (user3_name[i]!=0xff&&user3_pass[i]!=0xff)){
											total_users='1';
										}//case 4
										if((user1_name[i]!=0xff&&user1_pass[i]!=0xff)&& (user2_name[i]!=0xff&&user2_pass[i]!=0xff)  && (user3_name[i]==0xff&&user3_pass[i]==0xff)){
															total_users='2';
														}//case 5

										if((user1_name[i]==0xff&&user1_pass[i]==0xff) && (user2_name[i]!=0xff&&user2_pass[i]!=0xff&&user3_name[i]==0xff) && (user3_pass[i]==0xff)){
															total_users='1';
														}//case 6

										if((user1_name[i]!=0xff&&user1_pass[i]!=0xff) && (user2_name[i]==0xff&&user2_pass[i]==0xff) && (user3_name[i]==0xff&&user3_pass[i]==0xff)){
																total_users='1';
															}//case 7
															if((user1_name[i]==0xff&&user1_pass[i]==0xff)  && (user2_name[i]==0xff&&user2_pass[i]==0xff) && (user3_name[i]==0xff&&user3_pass[i]==0xff)){
																				total_users='0';
																			}//case 8

				}



}
void init_fast_pwm()
{
	    // fast PWM mode 15
		DDRD|=(1<<PD4);                     // oc1b as output
		TCCR1A|=(1<<WGM10)|(1<<WGM11);     //for mode 15 fast PWM
		TCCR1B|=(1<<WGM12)|(1<<WGM13);    //for mode 15 fast PWM
		TCCR1A|=(1<<COM1B1);             // for non inverted mode
		TCCR1B|=(1<<CS10)|(1<<CS11);    // prescaler over 64  (8000000/64=125000 hz)
		//****************************************************
		OCR1AH=0xff;  //  ocr1a ,  from data sheet, is top most value
		OCR1AL=0xff; //   ocr1a ,  from data sheet, is top most value
		// note: now frequency becomes 125000/(2^16)=1.907346 hz
		//that mean that output frqueny =1.907346 hz
		//speed of motor is slow
		CLEAR_BIT(DDRD,DDD5);
}
void PWM_init() {
    // Set Fast PWM mode with non-inverting output on pin OC1A (PD5)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << WGM12) | (1 << CS11); // Set prescaler to 8
    DDRD |= (1 << DDD5); // Set OC1A (PD5) pin as output
    CLEAR_BIT(DDRD,DDD4);
}
void duty_cycle_update(unsigned int duty_cycle)
{
	unsigned long ocrb=0;
	ocrb=(((duty_cycle*65536)/100)-1);
    OCR1BH=(ocrb>>8);
    OCR1BL=ocrb;

}
void PWM_set_duty_cycle(uint16_t duty_cycle) {
    OCR1A = duty_cycle;
}
void AC_Control()
{

	adcA = (int)(read_adc(5)*5.0)/1024.0;
	temperature=adcA/0.01;
	while(1)
	{
	LCD_IntToString((int) temperature,tempSTR);
	LCD_Send_String_Row_Column(1,0,"Temperature");
	LCD_Send_String_Row_Column(1,13,tempSTR);
	if(temperature>28)
	{

		//ON
		
		DDRD|=(1<<PD4);
		PORTD|=(1<<PD4);
		temperature--;
		_delay_ms(500);
		LCD_IntToString((int) temperature,tempSTR);
		LCD_Send_String_Row_Column(1,0,"Temperature");
		LCD_Send_String_Row_Column(1,13,tempSTR);
	}
	else if(temperature<21)
	{
		//OFF
		DDRD&=~(1<<PD4);
		PORTD&=~(1<<PD4);

		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		CLEAR_BIT(TCCR1B,WGM11);
		CLEAR_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
		duty_cycle_update(0);
		   break;
	}
	else
	{

		DDRD|=(1<<PD4);
		PORTD|=(1<<PD4);
		temperature--;
		_delay_ms(500);
		LCD_IntToString((int) temperature,tempSTR);
		LCD_Send_String_Row_Column(1,0,"Temperature");
		LCD_Send_String_Row_Column(1,13,tempSTR);
	}
	}
}
void Dimmer_Control()
{PWM_init();
	LCD_Clear_Display();
	if(admin_state==1)
	{
		LCD_Send_String_Row_Column(0,0,"Dimmer On");
	}
	else{
		LCD_Send_String_Row_Column(0,0,"Dimmer On");
	}
 	  while (1) {
        // Vary the brightness from 0 to 1023 and then back to 0
        for (brightness = 0; brightness < 1023; brightness++) {
            PWM_set_duty_cycle(brightness);
            _delay_ms(5); // Adjust the delay as needed for the desired brightness change speed
        }
        for (brightness = 1023; brightness > 0; brightness--) {
            PWM_set_duty_cycle(brightness);
            _delay_ms(5); // Adjust the delay as needed for the desired brightness change speed
        				}
    	CLEAR_BIT(DDRD,DDD5);
    	LCD_Clear_Display();
        	break;
		}
}
void buzzer()
{
	SET_BIT(DDRA,PA4);
	SET_BIT(PORTA,PA4);
	_delay_ms(3000);
	//system shutdown
	CLEAR_BIT(PORTA,PA4);
}
