/* 
Author       : Bent Furevik
Company      : University of Oslo
File name    : fys3240_lab2.c
Date         : 23.12.2011
Project      : FYS3240 Lab2
Function     : Precode for a microcontrollerprogram interfacing the LCD
*/

#include <avr/io.h>
#include "fys3240_lcd.h"


#define MAX_COUNTER_VAL 20

volatile unsigned long counter = 0;

void (*intCallbackFunction)(void) = 0;


void lcd_write_14seg(uint8_t addrSeg14, uint8_t data){
	if((addrSeg14>=0) && (addrSeg14<=6) ){
		LCD.CTRLG = LCD_TDG_14S_4C_gc | (((START_14SEG_4C+(6-addrSeg14)*4)<<LCD_STSEG_gp) & LCD_STSEG_gm);
		lcd_write_char(data);
	}

}

void lcd_write_7seg(uint8_t addrSeg7, uint8_t data)
{
	if((addrSeg7>=0) && (addrSeg7<=6) ){
		LCD.CTRLG = LCD_TDG_7S_4C_gc | (((START_7SEG_4C+(addrSeg7)*4)<<LCD_STSEG_gp) & LCD_STSEG_gm);
		LCD.CTRLH = (data);
	}
}


int main()
{
	init_32M_clock();
	
	//enable all the interrupt levels
	CCPWrite( &PMIC.CTRL, PMIC_HILVLEX_bm | PMIC_MEDLVLEX_bm | PMIC_LOLVLEX_bm );
	
	init_buttons_and_leds();
	init_lcd();
	init_lcd_interrupts();
	//enable gobal interrupt
	sei();
	
	//asm("sei"); //enable global interrupt
	//asm("cli"); //disable global interrupt
	
	
	
	LCD.CTRLG = LCD_TDG_14S_4C_gc | (((START_14SEG_4C+(6-6)*4)<<LCD_STSEG_gp) & LCD_STSEG_gm);
	lcd_write_char('U');
	lcd_write_char('I');
	lcd_write_char('O');
	
	lcd_set_blink(LCD_BLINKRATE_1Hz_gc);
	lcd_enable_blink(1);
	_delay_ms(3000);
	lcd_enable_blink(0);
	
	//return 0;
	
	lcd_write_14seg(6, 'F');
	asm("nop");
	lcd_write_14seg(5, 'Y');
	asm("nop");
	lcd_write_14seg(4, 'S');
	asm("nop");
	lcd_write_14seg(3, '4');
	asm("nop");
	lcd_write_14seg(2, '2');
	asm("nop");
	lcd_write_14seg(1, '4');
	asm("nop");
	lcd_write_14seg(0, '0');
	asm("nop");
	//lcd_write_7seg(0,'9');
	asm("nop");
	

	
	//lcd_write_int(1234567);
	lcd_write_int(6431259);
	lcd_backlight(1);
	/* Inits */
	
	while(1){
		/* Test-app */

		uint8_t buttonStatus = getButtonInput();
		//while(buttonStatus == getButtonInput()); 
		//_delay_ms(100);
		put_on_leds(buttonStatus);

		
		switch(~buttonStatus&0x0F){
			case 0x01: {
				counter = 0;
				break;
				};
			case 0x02: {
				counter++;
				if(counter > MAX_COUNTER_VAL) counter = 0;
				break;
			};
			case 0x04: {
				break;
			};
			case 0x08: {
				break;
			};
			default: {
				break;
			};
		}
		
		while(buttonStatus == getButtonInput()); 
		
	}
}



void lcd_enable_blink(int enable){
	/**/
	
	if(enable) LCD.CTRLD |= LCD_BLINKEN_bm;
	else LCD.CTRLD &= ~LCD_BLINKEN_bm;
	
	
}

void lcd_set_blink(LCD_BLINKRATE_t rate){
	/**/
	LCD.CTRLD &= ~LCD_BLINKRATE_gm;
	LCD.CTRLD |= rate;
	
	
}

void lcd_write_char_pos(const uint8_t data, uint8_t pos) 
{
	/**/
	lcd_write_14seg(pos,data);
}

void lcd_write_char(const uint8_t data) 
{
	/**/
	LCD.CTRLH = (data);

}

void lcd_write_int(long i) 
{
	/**/

	if(i<=9999999){
	
		asm("nop");
		char buff[7] ;
		ltoa ((long)i, buff, 10);
		int len = strlen(buff);




#if (USE_LCD_14SEG_NUM_ORIENTATION_RIGHT == 0)

//LEFT ORIENTED
		for(int c=0;c<=6;c++){	
			if(c<len){
				lcd_write_14seg(6-c, buff[c]);
			}
			else{
				lcd_write_14seg(6-c, 0);
			}	
		}
#elif (USE_LCD_14SEG_NUM_ORIENTATION_RIGHT == 1)		
	
//RIGHT ORIENTED
	for(int c=6;c>=0;c--){
		
		if(c>len){
			lcd_write_14seg(c, 0);
		}
		else{
			lcd_write_14seg(c, buff[len-c-1]);
		}
	}
#endif
	
	
	
	}


	
	
	
	

	
	
	
}


void init_lcd(void)
{
	/**/


	//enable the lcd and the segments
	LCD.CTRLA = LCD_ENABLE_bm | LCD_SEGON_bm;
	
	//enable the prescale (N=16) , set the clock division to /2 leading to 62.5Hz (page 312 of xmega datasheet)
	//also enable low power waveform (will test later if needed or not)
	LCD.CTRLB = LCD_PRESC_bm | LCD_CLKDIV1_bm |LCD_CLKDIV0_bm | LCD_LPWAV_bm;
	
	//LCD port mask value. (NEEDS FURTHER INVESTIGATION) (page 313)
	LCD.CTRLC = 0x28;
	
	//fine contrast value. Vlcd = 3V + FCONT x 0.016V).  set to max
	LCD.CTRLF = 0x3F;
	
	//setting LCD backlight pin as output
	PORTE.DIRSET = PIN5_bm;

	
	

}
/* initialize the lcd interrupt 
 *	initialize the lcd interrupt by setting its interrupt level and also setting the interrupt callback
 */
void init_lcd_interrupts(void){
	
	//set the lcd interrupt at High level
	CCPWrite(&LCD.INTCTRL,32<<3 | LCD_FCINTLVL_HI_gc );
	
	//set the pointer 'intCallbackFunction' for callback to the function 'lcd_callback_lab2'
	set_lcd_callback(lcd_callback_lab2);
}



/*
 *	the actual interrupt calllback function that is called from the IR
 */
void lcd_callback_lab2(void)
{

	lcd_write_int(counter); 
	
}







/* set the callback function to the pointer
 *	@param pointer2Func this parameter is the address of the function that is to be called inside the ISR
 */
void set_lcd_callback(void *pointer2Func){
	
	intCallbackFunction = pointer2Func;

}


void lcd_backlight(int status){
	if(status == 0){
		PORTE.OUTCLR = PIN5_bm;
	}
	else{
		PORTE.OUTSET = PIN5_bm;
	}
	
}



/* LCD interrupt function
 *	This is the interrupt vector function identified by the avr tool-chain for the LCD interrupt
 */
ISR(LCD_INT_vect){
	
	//callback to the interrupt function 'set_lcd_callback' via the pointer 'intCallbackFunction'
	intCallbackFunction();
	

}

