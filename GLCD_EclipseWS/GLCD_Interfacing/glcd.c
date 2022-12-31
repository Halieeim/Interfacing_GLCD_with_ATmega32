 /******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: glcd.c
 *
 * Description: Source file for the GLCD driver
 *
 * Author: Ahmed Abd El-Halim Ahmed
 *
 *******************************************************************************/

#include "glcd.h"
#include "util/delay.h"

#define FontWidth	128


void GLCD_command(uint8 command){
	GPIO_writePort(GLCD_DATA_PORT, command);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_LOW);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);
	_delay_us(5);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_LOW);
	_delay_us(5);
}

void GLCD_init(){
	/* setting up the direction of data port to be output port */
	GPIO_setupPortDirection(GLCD_DATA_PORT, PIN_OUTPUT);
	/* turn it off at the beginning */
	GPIO_writePort(GLCD_DATA_PORT, LOGIC_LOW);

	/* setting up the direction of control port to be output port */
	GPIO_setupPortDirection(GLCD_CONTROL_PORT, PIN_OUTPUT);

	/* turn it off at the beginning */
	GPIO_writePort(GLCD_CONTROL_PORT, LOGIC_LOW);

	/* CS1 = 1, CS2 = 1, RST = 1*/
	GLCD_selectPage(BOTH_PAGES);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RST_PIN, LOGIC_HIGH);

	_delay_ms(20);

/*
	GLCD_sendCommand(GLCD_DISPLAY_OFF, 0);
	GLCD_sendCommand(GLCD_SET_Y_ADDRESS, 0x40);
	GLCD_sendCommand(GLCD_SET_X_ADDRESS, 0xB8);
	GLCD_sendCommand(GLCD_SET_Z_ADDRESS, 0xC0);
	GLCD_sendCommand(GLCD_DISPLAY_ON, 0);*/

	GLCD_command(0x3E);		/* Display OFF */
	GLCD_command(0x40);		/* Set Y address (column=0) */
	GLCD_command(0xB8);		/* Set x address (page=0) */
	GLCD_command(0xC0);		/* Set z address (start line=0) */
	GLCD_command(0x3F);		/* Display ON */
}

void GLCD_selectPage(uint8 page){
	/* if right page is selected, then setting cs2 = 1, cs1 = 0 */
	/* if left page is selected, then setting cs2 = 0, cs1 = 1 */
	/* if both pages is selected, then setting cs2 = 1, cs1 = 1 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS2_PIN, GET_BIT(page,1));
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS1_PIN, GET_BIT(page,0));
}

void GLCD_sendCommand(uint16 command, uint8 data){
	/* RS = 0, R/W = 0 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_LOW);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);
	_delay_ms(1);
	switch(command){
	case GLCD_DISPLAY_ON:
		/* writing on data bus */
		GPIO_writePort(GLCD_DATA_PORT, command);
		break;

	case GLCD_DISPLAY_OFF:
		/* writing on data bus */
		GPIO_writePort(GLCD_DATA_PORT, command);
		break;

	case GLCD_SET_Y_ADDRESS:
		/* writing y address (0-63) on DB5 - DB0 */
		data &= ~(0xC0);	/* making sure it less than 64 */
		GPIO_writePort(GLCD_DATA_PORT, data);
		GPIO_writePin(GLCD_DATA_PORT, 6, LOGIC_HIGH);
		break;

	case GLCD_SET_X_ADDRESS:
		/* writing x address (0-7) on DB5 - DB0 */
		GPIO_writePort(GLCD_DATA_PORT, GLCD_SET_X_ADDRESS);
		GPIO_writePin(GLCD_DATA_PORT, 0, GET_BIT(data,0));
		GPIO_writePin(GLCD_DATA_PORT, 1, GET_BIT(data,1));
		GPIO_writePin(GLCD_DATA_PORT, 2, GET_BIT(data,2));
		break;

	case GLCD_SET_Z_ADDRESS:
		data &= ~(0xC0);	/* making sure it less than 64 */
		GPIO_writePort(GLCD_DATA_PORT, data);
		/* writing one on BD6,BD7 */
		GPIO_writePin(GLCD_DATA_PORT, 6, LOGIC_HIGH);
		GPIO_writePin(GLCD_DATA_PORT, 7, LOGIC_HIGH);
		break;

	case GLCD_WRITE_DISPLAY_DATA:
		/* RS = 1, R/W = 0 */
		GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_HIGH);
		GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);
		/* writing data on data bus */
		GPIO_writePort(GLCD_DATA_PORT, data);
		break;

	case GLCD_READ_DISPLAY_DATA:
		/* RS = 1, R/W = 1 */
		GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_HIGH);
		GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_HIGH);
		/* writing data on data bus */
		GPIO_writePort(GLCD_DATA_PORT, data);
		break;
	}
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);
	_delay_ms(1);
}

void GLCD_displayCharacter(uint8 data){
	GPIO_writePort(GLCD_DATA_PORT, data);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_HIGH);			/* Data Mode RS=1 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);			/* R/W = 0 */
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);			/* EN=1 */
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT,GLCD_E_PIN,LOGIC_LOW); 				/* Disable GLCD E=0 */
	_delay_ms(1); /* delay for processing */
}

void GLCD_displayString(const char *str){
	uint8 i = 0;
	while(str[i] != '\0')
	{
		GLCD_displayCharacter(str[i]);
		i++;
	}
}

void GLCD_displayStringPageColumn(const char *str, uint8 row, uint8 column, uint8 page){
	uint8 i, col, Y_Address = 0;
	float32 page_inc = 0.5;
	page = 0xB8 + page;
	GLCD_selectPage(LEFT_HALF_PAGE);
	GLCD_sendCommand(GLCD_SET_X_ADDRESS, page);
	GLCD_displayString(str);

}

void GLCD_diplayImage(long img[]){

}
void GLCD_diplayGIF(long **images){

}
void GLCD_clearScreen(){
	uint8 i, j;
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS1_PIN, LOGIC_HIGH);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS2_PIN, LOGIC_HIGH);

	for(i = 0; i < NO_PAGES; i++){
		GLCD_sendCommand(GLCD_SET_X_ADDRESS, 0xB8 + i);
		for (j = 0; j < 64; j++) {
			GLCD_displayCharacter(0);
		}
		GLCD_sendCommand(GLCD_SET_Y_ADDRESS, 0x40);
		GLCD_sendCommand(GLCD_SET_X_ADDRESS, 0xB8);
	}
}
