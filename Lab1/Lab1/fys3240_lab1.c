/* 
Author       : Bent Furevik/Snorre Bjørnstad
Company      : University of Oslo
File name    : fys3240_lab1.h
Date         : 23.12.2011
Revised		 : 02.02.2015
Project      : FYS3240 Lab1 
Function     : Code for a microcontrollerprogram flashing leds according to pushbuttons 
*/


#include "fys3240_led.h"

#include <avr/io.h>
#include <inttypes.h>

/* setting port direction as output
 * @param p the port to be configured as output
 * @param msk the mask value of bits to be set
 */
void port_dir_out(PORT_t *p, uint8_t msk)
{
	p->DIRSET = msk;
}


/* setting port direction as input
 * @param p the port to be configured as input
 * @param msk the mask value of bits to be set
 */
void port_dir_in(PORT_t *p, uint8_t msk)
{
	
	p->DIRCLR = msk;
}



/* write the port value
 * @param p the port to be configured as input
 * @param val value of the port
 */
void write_port_value(PORT_t *p, uint8_t val, uint8_t msk)
{
	
	p->OUTSET = msk;
	p->OUTCLR = (val)& msk;
	
}

/* read the port value
 * @param p the port to be configured as input
 * @return returns the value read at the port
 */
uint8_t read_port_value(PORT_t *p)
{
	return p->IN;
}




void CCPWrite( volatile uint8_t * address, uint8_t value )
{


	uint8_t volatile saved_sreg = SREG; 
			asm ("cli");


	volatile uint8_t * tmpAddr = address;
	#ifdef RAMPZ
	RAMPZ = 0;
	#endif
	asm volatile(
	"movw r30,  %0"       "\n\t"
	"ldi  r16,  %2"       "\n\t"
	"out   %3, r16"       "\n\t"
	"st     Z,  %1"       "\n\t"
	:
	: "r" (tmpAddr), "r" (value), "M" (CCP_IOREG_gc), "i" (&CCP)
	: "r16", "r30", "r31"
	);


	//leave critical section by restoring the SREG 
	SREG = saved_sreg;

}


/* enabling the 32 MHz clock
 */
void init_32M_clock(void)
{
	//enable the 32MHz oscillator
	OSC.CTRL |= OSC_RC32MEN_bm;
	
	//wait until the oscillator is steady 
	do {} while ((OSC.STATUS & (OSC_RC32MRDY_bm)) == 0);
		
	//code protect write into clock control register to set the clock source as 32MHz	
	CCPWrite( &CLK.CTRL, ( CLK.CTRL & ~CLK_SCLKSEL_gm ) | CLK_SCLKSEL_RC32M_gc );
	
		
}


// void init_32M_clock(void)
// {
// 	/* Your code here! */
// 	OSC.CTRL = (1<<1);
// 	while((OSC.STATUS&(1<<1)) == 0);
// 	
// 	CCP = 0xD8;
// 	
// 	CLK.CTRL = 0b001;
// 	OSC.CTRL &= ~OSC_RC32MEN_bm;
// 	
// 	
// }

void put_on_leds(uint8_t value)
{
	/* Your code here! */
	uint8_t temp  = (value << 4 ) & 0xF0;
	
write_port_value(&PORTB, (~value)<<4, 0xF0);
/*write_port_value(&PORTB, temp, 0xF0);*/
	
	


	
	
}

uint8_t getButtonInput(void)
{
	/* Your code here. Remember to ignore the 4 most significant bits since the buttons are on the 4 LSB. */
	return read_port_value(&PORTE) & 0x0F;
}

void flash_on_keypress(void)
{
	//uint8_t buttonStatus = getButtonInput();
	//put_on_leds(getButtonInput());

	while (1)
	{
		uint8_t buttonStatus = getButtonInput();
		put_on_leds(getButtonInput());
		switch(~buttonStatus & 0x0F){
			case 0x08: {
				return;
			};
			default:{
			break;
			};
	}
}

		}

void led_counter(void)
{
	int counter = 0;
	while(1)
	{
			uint8_t buttonStatus = getButtonInput();

			put_on_leds(~counter);
			
			switch(~buttonStatus & 0x0F){
				case 0x01: {
					counter++;
					break;
				};
				case 0x02: {
					counter--;
					break;
				};
				case 0x04: {
					break;
				};
				case 0x08: {
					_delay_ms(1000);
					return;
				};
				default: {
					break;
					
				};
			}
	_delay_ms(1000);
	
	}
	

	

}
				

void init_buttons(void)
{
	/* Initialize configuration of buttons */
	port_dir_in(&PORTE, PIN3_bm | PIN2_bm | PIN1_bm| PIN0_bm);
	
	
	//using the feature of the multi pin configuration to configure bits [3:0] for pull up
	PORTCFG.MPCMASK = 0b00001111;
	
	//witing to any one of the pin control register will configure for all the pinc ses in MCPMASK
	PORTE.PIN0CTRL |= PORT_OPC_PULLUP_gc;

	
}

void init_leds(void)
{
	/* Initialize configuration of LEDs */
	port_dir_out(&PORTB , PIN7_bm | PIN6_bm | PIN5_bm | PIN4_bm);
	
	
}

void init_buttons_and_leds(void)
{
	init_buttons();
	init_leds();
}

