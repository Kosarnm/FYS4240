/* 
Author       : Bent Furevik
Company      : University of Oslo
File name    : fys3240_lcd.c
Date         : 23.12.2011
Project      : FYS3240 Lab1
Function     : Precode for the methods used to interface the LCD on the XMEGA-board
*/
#define F_CPU 32000000
#include "fys3240_led.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>





//! 14-segment with 4 commons terminals, start segment
#define START_14SEG_4C 12
#define START_7SEG_4C 8
//Definer disse makroene:
// #define LCD_HOME 
// #define LCD_CLEAR
#define LCD_BACKLIGHT_TOGGLE (PORTE.OUT ^= (1 << 5))

#define USE_LCD_14SEG_NUM_ORIENTATION_RIGHT 1

void lcd_enable_blink(int enable);
void lcd_set_blink(LCD_BLINKRATE_t rate);
void lcd_write_char_pos(const uint8_t data, uint8_t pos) ;
void lcd_write_char(const uint8_t data) ;
void lcd_write_int(long i) ;
void init_lcd(void);
void init_lcd_interrupts(void);
void lcd_callback_lab2(void);
void lcd_backlight(int status);


/* set the callback function to the pointer
 *	@param pointer2Func this parameter is the address of the function that is to be called inside the ISR
 */
void set_lcd_callback(void *pointer2Func);


