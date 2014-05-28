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

void flip_latch(void){};

void msg_error(void);

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
0b01100000, //'E' for error
};

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //disable watchdog
	
	//initialize stuff
	P1DIR |= ( MOSI | SCLK | SS );
	P2DIR |= ( ALL_DIGS );
	
	//infinite loop
	for(;;)
	{
		write_number(4398);
		__delay_cycles(1000);
	}
	return 0; //should never reach this point
}

void flip_latch(void){
	//flip the latch which is SS
	P1OUT &= ~(SS);
	P1OUT |= SS;
	
}

void bb_shift_out(unsigned char data){
	unsigned char j;
	for (j = 0; j < 8; j++){
		if ( data & (1 << j)){
			P1OUT |= MOSI; //set MOSI high
		} else {
			P1OUT &= ~(MOSI);
		}
		//pulse the clock
		P1OUT &= ~(SCLK); //LOW
		P1OUT |= SCLK; //HIGH
	}
}



void write_digit(unsigned char num, unsigned char dig){
	unsigned char k;
	bb_shift_out(number_seg_bytes[num]);
	for( k = 0; k < sizeof(digit_bits); k++){
		if ( k == dig ){
			P2OUT |= digit_bits[k];
		} else {
			P2OUT &= ~(digit_bits[k]);
		}
	}
	flip_latch();
	
}

void msg_error(void){
	write_digit(10, 0);
}

void write_number(unsigned char number){
		
		//check if number is too big ot not
		if (number < 10000){
			//formats number based on digits to correct digits on display
			write_digit((number/1000),3);
			write_digit((number%1000)/100,2);
			write_digit(((number%1000)%100)/10, 1 );
			write_digit((((number%1000)%100)%10), 0);           
		} else {
			msg_error();
		}
}

