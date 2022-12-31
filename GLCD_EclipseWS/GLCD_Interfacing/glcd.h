 /******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: glcd.h
 *
 * Description: Header file for the GLCD driver
 *
 * Author: Ahmed Abd El-Halim Ahmed
 *
 *******************************************************************************/

#ifndef GLCD_H_
#define GLCD_H_

#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"

/* GLCD total number of pages */
#define NO_PAGES							8

/* GLCD ports and pins configurations */
#define GLCD_DATA_PORT						PORTD_ID
#define GLCD_CONTROL_PORT					PORTC_ID
#define GLCD_RST_PIN						PIN0_ID
#define GLCD_E_PIN							PIN1_ID
#define GLCD_R_W_PIN						PIN2_ID
#define GLCD_RS_PIN							PIN3_ID
#define GLCD_CS2_PIN						PIN4_ID
#define GLCD_CS1_PIN						PIN5_ID

/* GLCD pages */
#define LEFT_HALF_PAGE						0x01	// cs2=0, cs1=1
#define RIGHT_HALF_PAGE						0x02	// cs2=1, cs1=0
#define BOTH_PAGES							0x03	// cs2=1, cs1=1

/* GLCD Commands */
#define GLCD_DISPLAY_ON						0x3F
#define GLCD_DISPLAY_OFF					0x3E
#define GLCD_SET_Y_ADDRESS					0x40
#define GLCD_SET_X_ADDRESS					0xB8
#define GLCD_SET_Z_ADDRESS					0xC0


void GLCD_init();
void GLCD_sendCommand(uint8 command);
void GLCD_selectPage(uint8 page);
void GLCD_moveCursor(uint8 row, uint8 col, uint8 page);
void GLCD_displayCharacter(uint8 dara);
/*void GLCD_displayString(const unsigned char *str);
void GLCD_displayStringPageColumn(const unsigned char *str, uint8 row, uint8 column, uint8 page);*/
void GLCD_diplayImage(const unsigned char *img);
void GLCD_diplayGIF(const unsigned char **images, uint8 No_Images);
void GLCD_clearScreen();




#endif /* GLCD_H_ */
