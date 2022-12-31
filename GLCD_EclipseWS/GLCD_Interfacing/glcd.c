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

uint8 j = 0, X_ADDRESS = GLCD_SET_X_ADDRESS;

void GLCD_init(){
	/* setting up the direction of data port to be output port */
	GPIO_setupPortDirection(GLCD_DATA_PORT, PORT_OUTPUT);
	/* turn it off at the beginning */
	GPIO_writePort(GLCD_DATA_PORT, LOGIC_LOW);

	/* setting up the direction of control port to be output port */
	GPIO_setupPortDirection(GLCD_CONTROL_PORT, PORT_OUTPUT);

	/* turn it off at the beginning */
	GPIO_writePort(GLCD_CONTROL_PORT, LOGIC_LOW);

	/* CS1 = 1, CS2 = 1, RST = 1*/
	GLCD_selectPage(LEFT_HALF_PAGE);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RST_PIN, LOGIC_HIGH);

	_delay_ms(20);

	GLCD_sendCommand(GLCD_DISPLAY_OFF);			/* Display OFF */
	GLCD_sendCommand(GLCD_SET_Y_ADDRESS);		/* Set Y address (column=0) */
	GLCD_sendCommand(GLCD_SET_X_ADDRESS);		/* Set x address (page=0) */
	GLCD_sendCommand(GLCD_SET_Z_ADDRESS);		/* Set z address (start line=0) */
	GLCD_sendCommand(GLCD_DISPLAY_ON);			/* Display ON */
}

void GLCD_selectPage(uint8 page){
	/* if right page is selected, then setting cs2 = 1, cs1 = 0 */
	/* if left page is selected, then setting cs2 = 0, cs1 = 1 */
	/* if both pages is selected, then setting cs2 = 1, cs1 = 1 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS2_PIN, GET_BIT(page,1));
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS1_PIN, GET_BIT(page,0));
}

void GLCD_sendCommand(uint8 command){
	GPIO_writePort(GLCD_DATA_PORT, command);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_LOW);	/* RS=0 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);  /* R/W=0 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);	/* EN=1 */
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_LOW);	/* EN=0 */
	_delay_ms(1);
}

void GLCD_displayCharacter(uint8 data){
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_RS_PIN, LOGIC_HIGH);			/* Data Mode RS=1 */
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_R_W_PIN, LOGIC_LOW);			/* R/W = 0 */
	GPIO_writePort(GLCD_DATA_PORT, data);
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_E_PIN, LOGIC_HIGH);			/* EN=1 */
	_delay_ms(1);
	GPIO_writePin(GLCD_CONTROL_PORT,GLCD_E_PIN,LOGIC_LOW); 				/* Disable GLCD E=0 */
	_delay_ms(1); /* delay for processing */
}

void GLCD_displayImage(const unsigned char *str){
	uint16 i = 0;
	while(i < 1024)
	{
		if (j == 64 ) {
			GLCD_selectPage(RIGHT_HALF_PAGE);
			GLCD_sendCommand(X_ADDRESS);
		} else if (j == 128){
			X_ADDRESS++;
			GLCD_selectPage(LEFT_HALF_PAGE);
			GLCD_sendCommand(X_ADDRESS);
			j = 0;
		}
		GLCD_displayCharacter(str[i]);
		i++;
		j++;
	}
}

void GLCD_diplayGIF(const unsigned char **images, uint8 NO_Images){
	uint8 i;
	for(i = 0; i < NO_Images; i++){
		GLCD_displayImage(images[i]);
	}
}

void GLCD_clearScreen(){
	uint8 i, j;
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS1_PIN, LOGIC_HIGH);
	GPIO_writePin(GLCD_CONTROL_PORT, GLCD_CS2_PIN, LOGIC_HIGH);

	for(i = 0; i < NO_PAGES; i++){
		GLCD_sendCommand(0xB8 + i);
		for (j = 0; j < 64; j++) {
			GLCD_displayCharacter(0);
		}
	}
	GLCD_sendCommand(0x40);
	GLCD_sendCommand(0xB8);
}
