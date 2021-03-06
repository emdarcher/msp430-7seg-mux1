//program to display an analog value on
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
void write_number(unsigned int number);

void bb_shift_out(unsigned char data);

void flip_latch(void);

void msg_error(void);

//add any defined digits to this array
unsigned char digit_bits[] = { DIG_0, DIG_1, DIG_2, DIG_3 };
unsigned char num_digits = sizeof(digit_bits);

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
	P1OUT |= SS;
	//infinite loop
	//bb_shift_out(0xFF);
	for(;;)
	{
		//msg_error();
		write_number(2345);
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
	for (j = 0; j < 8 ; j++){
		if ( !(data & (1 << (7-j)))){
			P1OUT |= MOSI; //set MOSI high
		} else {
			P1OUT &= ~(MOSI);
		}
		//pulse the clock
		P1OUT &= ~(SCLK); //LOW
		//__delay_cycles(100);
		P1OUT |= SCLK; //HIGH
	}
}



void write_digit(unsigned char num, unsigned char dig){
	unsigned char k;
	unsigned char p2stuff;
	if(num < 10){
	bb_shift_out(number_seg_bytes[num]);
	} else {bb_shift_out(number_seg_bytes[10]);}
	//flip_latch();
	for( k = 0; k < num_digits; k++){
		if ( k == dig ){
			p2stuff |= digit_bits[k];
		} else {
			p2stuff &= ~(digit_bits[k]);
		}
	}
	P2OUT = p2stuff;
	flip_latch();
	//__delay_cycles(5000);
}

void msg_error(void){
	write_digit(10, 0);
}

void write_number(unsigned int number){
		unsigned char h;
		unsigned int format_num = number;
		//check if number is too big ot not
		if (number < 10000){
			//formats number based on digits to correct digits on display
			for(h=0;h < num_digits;h++){
				write_digit(format_num % 10, h);
				format_num /= 10;
			}         
		} else {
			msg_error();
		}
}

