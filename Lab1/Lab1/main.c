/*
 * Lab1.c
 *
 * Created: 01.02.2019 15:37:29
 * Author : kosarn
 */ 

#include <avr/io.h>

#include "fys3240_led.h"







int main(void)
{
	
	asm("nop");
	asm("nop");
	
	init_32M_clock();
	
	init_buttons_and_leds();
	
	
	//put_on_leds(0b1010);
    /* Replace with your application code */
	//init_leds();
	
	
	led_counter();
	flash_on_keypress();
	while(1)
	{
	}
	
	

}

