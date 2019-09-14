/* 
Author       : Bent Furevik
Company      : University of Oslo
File name    : fys3240_adc.h
Date         : 23.12.2011
Project      : FYS3240 Lab3
Function     : Precode for a microcontrollerprogram interfacing the ADC
*/

/* We will reuse the code from lab 1 and 2. */
#define F_CPU 32000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "fys3240_lcd.h"
#include "fys3240_led.h"

volatile uint8_t val;

void ADCBInit(void) ;
void adcb_callback(void);
void lcd_for_adc_callback(void);
static uint8_t conv_ascii(uint8_t val);
void show_value(uint8_t val);