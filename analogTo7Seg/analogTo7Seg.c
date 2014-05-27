//program to display an analog value from A1 on
//4 7-segment displays the are multiplexed.

//this one will use bit banged SPI
//next revision will attempt using SPI on the USI

#include <msp430.h>
#include "pinHeader.h"

//other defines

//SPI pins
#define SS BIT4 // RCK or Latch for shift register
#define MOSI BIT6 // SER IN or Data on shift register
#define SCLK BIT5 // SRCK or Clock on shift register

//prototypes
void clear_display(void);
void write_digit(unsigned char num, unsigned char dig);
void write_number(unsigned char number);

void bb_shift_out(unsigned char data);


//add any defined digits to this array
unsigned char digit_bits[] = { DIG_0, DIG_1, DIG_2, DIG_3 };
//int i used for "for" loops
int i;

//array to store bytes of port 1 values to make numbers
unsigned char number_seg_bytes[] = {
//       unconfigured
//ABCDEFG^
0b00000010,//0
0b10011110,//1
0b00100100,//2
0b00001100,//3
0b10011000,//4
0b01001000,//5
0b01000000,//6
0b00011110,//7
0b00000000,//8
0b00011000,//9

};

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//infinite loop
	for(;;)
	{
	
	
	}
	return 0; //should never reach this point
}
