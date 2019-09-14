/*
Author       : Bent Furevik/Snorre Bjørnstad
Company      : University of Oslo
File name    : fys3240_lab1.h
Date         : 23.12.2011
Revised		 : 02.02.2015
Project      : FYS3240 Lab1
Function     : Code for a microcontrollerprogram flashing leds according to pushbuttons
*/

#define F_CPU 32000000UL

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


#define DEVICE ATXMEGA128B1



#define LED_PORT			PORTB
#define LED_PORT_MASK		(PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm)
#define BUTTON_PORT			PORTE
#define BUTTON_PORT_MASK	(PIN3_bm | PIN2_bm | PIN1_bm| PIN0_bm)


void init_32M_clock(void);
void put_on_leds(uint8_t value);
uint8_t getButtonInput(void);
void flash_on_keypress(void);
void led_counter(void);
void init_buttons(void);
void init_leds(void);
void init_buttons_and_leds(void);

/* code protection write
 * @param address the code protected register address to be written
 * @param value the value to be written into the code protection register
 */
void CCPWrite( volatile uint8_t * address, uint8_t value );