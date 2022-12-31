 /******************************************************************************
 *
 * File Name: main.c
 *
 * Description: Source file for interfacing GLCD with AVR
 *
 * Author: Ahmed Abd El-Halim Ahmed
 *
 *******************************************************************************/

#include "glcd.h"
#include "util/delay.h"

int main(void)
{
	// initialize GLCD
	GLCD_init();
	GLCD_displayCharacter('A');
	GLCD_displayCharacter('A');
	GLCD_displayCharacter('A');
	GLCD_displayCharacter('S');

	for(;;)
	{
		// write your code here
	}
}
