this is the new and improved code that uses the USI peripheral to do Hardware assisted SPI communication to the shift registers. This method achieves a much greater speed.

Also this code uses LPM0 when idle and wakes up every 0.512ms to send a new update to the display.
