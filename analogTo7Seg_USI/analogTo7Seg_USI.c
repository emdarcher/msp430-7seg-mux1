//program to display an analog value from A1 on
//4 7-segment displays the are multiplexed.

//this one will use the USI for SPI
//

#include <msp430.h>
#include <stdint.h>

//look in this header file "pinHeader.h" to see ascii visuals
//of the 7 segment display pinouts and also see the pins 
//used for the various controls on the msp430 mcu
#include "pinHeader.h"

//other defines

//SPI pins
#define SS BIT4 // RCK or Latch for shift register
#define MOSI BIT6 // SER IN or Data on shift register
#define SCLK BIT5 // SRCK or Clock on shift register

#define A1  BIT1


//prototypes
void clear_display(void);
void write_digit(unsigned char num, unsigned char dig);
void write_number(unsigned int number);

void bb_shift_out(unsigned char data);

void flip_latch(void);

void msg_error(void);

void ADC_init(void);

void USI_init(void);

void WDT_init(void);

unsigned int ADC_read_A1(void);

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
	//WDTCTL = WDTPW + WDTHOLD; //disable watchdog
    WDT_init();
    
	//initialize stuff
	P1DIR |= ( MOSI | SCLK | SS );
	P2DIR |= ( ALL_DIGS );
	P1OUT |= SS;
	
    BCSCTL1 = CALBC1_1MHZ;          // Running at 1 MHz
    DCOCTL = CALDCO_1MHZ;
    
    USI_init();
    
    ADC_init();
    
    
	unsigned int value = 0;
	//infinite loop
	for(;;)
	{
		value = ADC_read_A1();
		write_number(value);
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
	
	for( k = 0; k < num_digits; k++){
		if ( k == dig ){
			p2stuff |= digit_bits[k];
		} else {
			p2stuff &= ~(digit_bits[k]);
		}
	}
	P2OUT = p2stuff;
	flip_latch();
	
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

void WDT_init(void){
    
    WDTCTL = ( WDTPW | WDTTMSEL | WDTCNTCL | WDTIS1 );
    //initialize watchdog timer as an interval timer. WDTTMSEL
    //clear it. WDTCNCTL
    //set up with SMCLK/div 512, 1000000/512=1953.125Hz 0.512ms: WDTIS1
    IE1 |= WDTIE; //enable interrupt
    
}

void USI_init(void){
    
    //enable SDO, enable SCLK, master mode, output enabled, reset USI.
    USICTL0 = ( USIPE6 | USIPE5 | USIMST | USIOE | USISWRST );
    
    //CKPH = 1 , interrupt enable for USI counter,
    USICTL1 = ( USICKPH | USIIE );
    
    //div 111 so SMCLK / 8 = 125000Hz so 0.008ms for SCLK, select SMCLK
    USICKCTL = (USIDIV_3 | USISSEL_2);
    
    USICTL0 &= ~USISWRST; // Release USI from reset
    USICTL1 &= ~USIIFG; //avoid unwanted interrupt
}

//from mspsci.blogspot.com ADC10 tutorial
void ADC_init(void) {
            // Use Vcc/Vss for Up/Low Refs, 16 x ADC10CLKs, turn on ADC
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON;
            // A1 input, use ADC10CLK div 1, single channel mode  
    ADC10CTL1 =  INCH_1 + SHS_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_0;
    ADC10AE0 = A1;      // Enable ADC input on P1.1
    
    //ADC10CTL0 |= ENC+ ADC10SC;     // Enable conversions.
} // ADC_init

unsigned int ADC_read_A1(void){
	unsigned int v;
	ADC10CTL0 |= ENC+ ADC10SC;     // Enable conversions.
	while ((ADC10CTL1 & ADC10BUSY) == 0x01){
	}   // wait for conversion to end
	v=ADC10MEM;
	ADC10CTL0&=~ENC;                     //disable adc conv
	return v;
}

//interrupts

__attribute__((interrupt(WDT_VECTOR)))
void WDT_ISR(void){
    
    
    
}
