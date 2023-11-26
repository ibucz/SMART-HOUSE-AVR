/*
 * LCD.c
 *
 * Created: 11/06/2023
 * Author : AZIZ
 */
#include "LCD.h"
/*LCD initialization & clearing*/
void LCD_Init(void)
{
	LCD_CTRL_PORT_DIR |=(1<<E)|(1<<RS);

	LCD_DATA_PORT_DIR=0x0F;
	LCD_Send_Command(ReturnHome);
	LCD_Send_Command(_4BIT_2LINE);

	LCD_Send_Command(CLEAR_DISPLAY);
	LCD_Send_Command(DISPLAY_ON_CURSOR_OFF);
}

/*Sending A command to the LCD */
void LCD_Send_Command(unsigned char command)
{


	LCD_CTRL_PORT &=~(1<<RS); // command mode
	_delay_ms(1);
	LCD_CTRL_PORT |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =((command>>4)&0x0F);
	LCD_CTRL_PORT &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT &=~(1<<RS); // command mode
	LCD_CTRL_PORT |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =((command)&0x0F);
	LCD_CTRL_PORT &=~(1<<E);
	_delay_ms(1);

}
/*LCD Clearing the display*/
void LCD_Clear_Display(void)
{
	LCD_Send_Command(CLEAR_DISPLAY);
}

/*LCD displaying a character*/
void LCD_Send_character(char character)
{
	/*LCD as 8bit mode*/
#if LCD_BIT_MODE == 8
	LCD_CTRL_PORT |=(1<<RS);// Data mode
	LCD_CTRL_PORT |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =character;
	_delay_ms(1);
	LCD_CTRL_PORT &=~(1<<E);
	_delay_ms(1);

	/*LCD as 8bit mode*/
#elif LCD_BIT_MODE == 4
	LCD_CTRL_PORT |=(1<<RS); // Data mode
	_delay_ms(1);
	LCD_CTRL_PORT |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =((character>>4)& 0x0F);
	LCD_CTRL_PORT &=~(1<<E);
	_delay_ms(1);

	LCD_CTRL_PORT |=(1<<RS); // Data mode
	LCD_CTRL_PORT |=(1<<E);
	_delay_ms(1);
	LCD_DATA_PORT =(character & 0x0F);
	LCD_CTRL_PORT &=~(1<<E);
	_delay_ms(1);
#endif
}

/*LCD displaying a String*/
void LCD_Send_String(char *string)
{
	int i=0;
	while(string[i]!='\0')
	{
		LCD_Send_character(string[i]);
		i++;
	}
}

/*set the cursor at certain location*/
void LCD_GoTO_Row_Column(char Row,char Column)
{
	unsigned char address=0;

	switch (Row)
	{
	case 0:address =(Column);break;
	case 1:address =(Column+0x40);break;
	case 2:address =(Column+0x10);break;
	case 3:address =(Column+0x50);break;
	}

	/* we need to apply the corresponding command 0b10000000+Address */
	LCD_Send_Command(address|(SET_CURSOR_LOCATION));
}

/*lcd Send String at certain location*/
void LCD_Send_String_Row_Column(char Row,char Column,char *String)
{
	LCD_GoTO_Row_Column(Row,Column);
	LCD_Send_String(String);
}

/*since the lcd is alpha Numeric print characters only*/
void LCD_IntToString(int integer,char *String)
{
	itoa(integer,String,10);
}
