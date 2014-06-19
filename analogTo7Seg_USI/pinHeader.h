//header file to store the defines for connections and such

/*
segments
 ---A---
|       |
F       B
|___G___|
|       |
E       C
|___D___|

*/
/*

pinout of display
        ________     
C1    --| ---- |--  G1
E1    --|digit1|--  A1
D1    --| ---- |--  F1
Vcc1  --|      |--  B1
Vcc0  --| ---- |--  B0
D0    --|digit0|--  F0
E0    --| ---- |--  A0
C0    --|______|--  G0

common adode for each digit
pull segment's pin LOW to turn it on
*/

//defines

//these are the bits of the shift register
//that go to particular segments
#define SEG_A BIT7
#define SEG_B BIT6
#define SEG_C BIT5
#define SEG_D BIT4
#define SEG_E BIT3
#define SEG_F BIT2
#define SEG_G BIT1

#define ALL_SEGS ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G )

#define DIG_0 BIT0
#define DIG_1 BIT1
#define DIG_2 BIT2
#define DIG_3 BIT3


//remember to add any newly defines digits here
#define ALL_DIGS ( DIG_0 | DIG_1 | DIG_2 | DIG_3 )


