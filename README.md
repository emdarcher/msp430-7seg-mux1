 msp430 7-segment display multiplexing
=================

 analogTo7Seg
-------------
code that takes an analog input and displays the 10-bit value on two multiplexed 2-digit 7-segment displays.


display multiplexing using a TPIC6B595 power shift register
controlled by a TI msp430g2252 ic on a breadboard.


####7-segment display pinouts
    segments
     ---A---
    |       |
    F       B
    |___G___|
    |       |
    E       C
    |___D___|


    pinout of one display module
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
