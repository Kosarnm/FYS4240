/* 
Author       : Bent Furevik
Company      : University of Oslo
File name    : fys3240_lab3.c
Date         : 23.12.2011
Project      : FYS3240 Lab3
Function     : Precode for a microcontrollerprogram interfacing the ADC
*/



#include "fys3240_adcb.h"


/* ISR's here */



void (*ADCIntCallbackFunction)(void) = 0;

typedef enum {NTC = 0, LDR, POT}Sensor_t;

Sensor_t currentSensor;

volatile uint8_t val;
volatile uint16_t sensorValue;
volatile uint8_t selectedSensorIndicator = 0;

uint16_t getNTCvalue(){
	
	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN8_gc;
	ADCA.INTFLAGS = /*ADC_CH3IF_bm | ADC_CH2IF_bm | ADC_CH1IF_bm |*/ ADC_CH0IF_bm;
	ADCA.CTRLA |= ADC_CH0START_bm;
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	return ADCA.CH0RES;
}

uint16_t getLDRvalue(){

	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN10_gc;
	ADCA.INTFLAGS = /*ADC_CH3IF_bm | ADC_CH2IF_bm | ADC_CH1IF_bm |*/ ADC_CH0IF_bm;
	ADCA.CTRLA |= ADC_CH0START_bm;
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	return ADCA.CH0RES;
}


uint16_t getPotvalue(){

	ADCA.CH0.MUXCTRL |= ADC_CH_MUXPOS_PIN9_gc;
	ADCA.INTFLAGS = /*ADC_CH3IF_bm | ADC_CH2IF_bm | ADC_CH1IF_bm |*/ ADC_CH0IF_bm;
	ADCA.CTRLA |= ADC_CH0START_bm;
	while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
	return ADCA.CH0RES;
}


void selectSensor(Sensor_t sensor){
	currentSensor = sensor;
	switch(sensor){
		case NTC	: ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN8_gc; break;
		case LDR	: ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN10_gc;break;
		case POT	: ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN9_gc;break;
		default : break;
	}
}

void startAdcConversion(){
	ADCA.INTFLAGS = /*ADC_CH3IF_bm | ADC_CH2IF_bm | ADC_CH1IF_bm |*/ ADC_CH0IF_bm;
	ADCA.CTRLA |= ADC_CH0START_bm;
	
	//This below blocking code is not needed when using interrupt for ADC, or else it causes a stuck at loop
	//while(!(ADCA.INTFLAGS & ADC_CH0IF_bm));
}


/* set the callback function to the pointer
 *	@param pointer2Func this parameter is the address of the function that is to be called inside the ISR
 */
void set_adc_callback(void *pointer2Func){
	
	ADCIntCallbackFunction = pointer2Func;

}






int main(void)
{
	

	
	
	init_32M_clock();
	
	//enable all the interrupt levels
	CCPWrite( &PMIC.CTRL, PMIC_HILVLEX_bm | PMIC_MEDLVLEX_bm | PMIC_LOLVLEX_bm );
	
	init_buttons_and_leds();
	init_lcd();
	init_lcd_interrupts();

	
	
	ADCBInit();
	
	set_adc_callback(adcb_callback);
	
	//enable gobal interrupt
	sei();

	int tempsum;
	int tempsum_avg = 0;
	/* Test application here. */
    while(1){
	
		
		if(currentSensor == NTC){
			put_on_leds(0b1110);
		}
		else if(currentSensor == LDR){
			put_on_leds(0b1101);
		}
		else if(currentSensor == POT){
			put_on_leds(0b1011);
		}	
		
		startAdcConversion();
		


		_delay_ms(200);
	}
}





void ADCBInit(void) 
{
	/* Code for initializing ADCB here */
	ADCA.CTRLA |= ADC_ENABLE_bm; //enable adc
	
	
	ADCA.CTRLB |= ADC_CONMODE_bm | ADC_RESOLUTION_12BIT_gc;
		
	ADCA.REFCTRL |= ADC_REFSEL_INTVCC_gc;
		
	ADCA.PRESCALER |= ADC_PRESCALER_DIV4_gc;
	ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_START_bm;
	
	ADCA.CH0.INTCTRL |= ADC_CH_INTLVL_HI_gc;
	
	
	

}

void adcb_callback(void)
{

	/* Code for the callback function for adc-interrupts here. */
	sensorValue = ADCA.CH0RES;	

	
	uint8_t buttonStatus = getButtonInput();
	switch(~buttonStatus&0x0F){
		case 0x01: {
			currentSensor = NTC;
			selectSensor(NTC);
			break;
		};
		case 0x02: {
			currentSensor = LDR;
			selectSensor(LDR);
			break;
		};
		case 0x04: {
			currentSensor = POT;
			selectSensor(POT);
			break;
		};
		case 0x08: {
			break;
		};
		default: {
			break;
		};
	}

	//while(buttonStatus == getButtonInput());	
	
	
	
	
	
}

void lcd_for_adc_callback(void)
{

	
	
	lcd_write_int(sensorValue);
	
	
	/* Code for the callback function for lcd-interrupts here. */
}

static uint8_t conv_ascii(uint8_t val)
{
	/* Make this return an ascii-character of the number. */
	return 0;
}

void show_value(uint8_t val){
	/* Show value on LCD-screen. */
}

/* LCD interrupt function
 *	This is the interrupt vector function identified by the avr tool-chain for the LCD interrupt
 */
ISR(ADCA_CH0_vect){

	adcb_callback();
	

}
