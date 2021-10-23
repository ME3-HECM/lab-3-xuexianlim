#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{
    LATA = 0; //set initial output LAT values (they may have random values when powered on)
    LATB = 0; //initialise LAT before TRIS
    LATF = 0;
    LATG = 0;
	
    TRISA = 0; //set up TRIS registers for pins connected to LED array
	TRISB = 0;
    TRISFbits.TRISF0 = 0;
    TRISFbits.TRISF6 = 0;
    TRISG = 0;
}

void button_init(void)
{
    TRISFbits.TRISF2 = 1; //set TRIS value for RF2 input (button)
    ANSELFbits.ANSELF2 = 0; //disable analogue input
}

/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(unsigned int number)
{
    //some code to turn on/off the pins connected to the LED array
	if (number & 1)        {LATGbits.LATG0 = 1;} else {LATGbits.LATG0 = 0;} //test bit 0 against bit mask and set LED
    if (number & (1 << 1)) {LATGbits.LATG1 = 1;} else {LATGbits.LATG1 = 0;} //test bit 1
    if (number & (1 << 2)) {LATAbits.LATA2 = 1;} else {LATAbits.LATA2 = 0;} //...
    if (number & (1 << 3)) {LATFbits.LATF6 = 1;} else {LATFbits.LATF6 = 0;}
    if (number & (1 << 4)) {LATAbits.LATA4 = 1;} else {LATAbits.LATA4 = 0;}
    if (number & (1 << 5)) {LATAbits.LATA5 = 1;} else {LATAbits.LATA5 = 0;}
    if (number & (1 << 6)) {LATFbits.LATF0 = 1;} else {LATFbits.LATF0 = 0;}
    if (number & (1 << 7)) {LATBbits.LATB0 = 1;} else {LATBbits.LATB0 = 0;}
    if (number & (1 << 8)) {LATBbits.LATB1 = 1;} else {LATBbits.LATB1 = 0;} //test bit 8
}

/************************************
/ Function LEDarray_disp_dec
/ Used to display a number on the LEDs
/ where each LED is a value of 10
************************************/
void LEDarray_disp_dec(unsigned int number)
{
	unsigned int disp_val;
    
	//some code to manipulate the variable number into the correct
	//format and store in disp_val for display on the LED array
    if (number / 10 > 9) {disp_val = 0b111111111;} //saturation
    else {disp_val = 0b111111111 >> (9 - number / 10);} //turn off LEDs from the most significant bit based on the magnitude of number
    
	LEDarray_disp_bin(disp_val); 	//display value on LED array
}


/************************************
/ LEDarray_disp_PPM
/ Function used to display a level on the LED array with peak hold
/ cur_val is the current level from the most recent sample, and max is the peak value for the last second
/ these input values need to calculated else where in your code
************************************/
void LEDarray_disp_PPM(unsigned int cur_val, unsigned int max)
{
	unsigned int disp_val;
	
	// some code to format the variable cur_val and max, store in disp_val for display on the LED array
    if (cur_val / 10 > 9) {cur_val = 0b111111111;} //saturation
    else {cur_val = 0b111111111 >> (9 - cur_val / 10);} //turn off LEDs from the most significant bit based on the magnitude of cur_val
    
    if (max / 10 > 9) {max = 0b100000000;} //saturation
    else {max = 0b100000000 >> (9 - max / 10);} //right shift based on magnitude of max

    disp_val = cur_val | max;
    
	LEDarray_disp_bin(disp_val);	//display value on LED array
}