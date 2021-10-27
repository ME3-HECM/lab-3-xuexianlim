#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // 1:256
    T0CON0bits.T016BIT=1;	//8bit mode

    // enable interrupt
    PIE0bits.TMR0IE = 1; // timer 0
    PIR0bits.TMR0IF = 0; // clear interrupt flag
    IPR0bits.TMR0IP = 0; // low priority
    INTCONbits.IPEN = 1; // enable interrupt priority
    INTCONbits.PEIE = 1; // peripheral interrupt
    INTCONbits.GIE = 1; // global interrupt
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=0b00001011;            //write High reg first, update happens when low reg is written to
    TMR0L=0b11011011;
    T0CON0bits.T0EN=1;	//start the timer
}

/************************************
 * Function to return the full 16bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
************************************/
unsigned int get16bitTMR0val(void)
{
	//add your code to get the full 16 bit timer value here
    return TMR0L | (TMR0H << 8);
}

void __interrupt(low_priority) LowISR()
{   
    if (PIR0bits.TMR0IF == 1) { // check interrupt flag
        LATHbits.LATH3 = !LATHbits.LATH3; // toggle LED
        
        /* 4*PS/Fosc = 4*256/(64*10^6) = 1/62500 (TMR0 increments every 1/62500 s)
         * 2^16-1-62500 = 3035 (TMR0 can count up to 2^16 - 1)
         * 3035 = 0b00001011 11011011 (Initialise TMR0 to 3035 so 1 s passes each time it overflows)*/
        
        TMR0H = 0b00001011; // reset timer
        TMR0L = 0b11011011;
        PIR0bits.TMR0IF = 0; // clear interrupt flag
    }
}