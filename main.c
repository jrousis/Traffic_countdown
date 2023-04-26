/*
 * Traffic_countdown.c
 *
 * Created: 25/4/2023 8:04:16 μμ
 * Author : user
 */ 

#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

static const __flash char flash_fonts[] = {
		0b11111111,0b11111100,0b00111111,0b11111111,	//0
		0b00000000,0b00000000,0b00000011,0b11111111,	//1 0b11111111,0b11000000,0b00000000,0b00000000,
		0b00000111,0b11111111,0b11111111,0b11100000,	//2
		0b00000000,0b00111111,0b11111111,0b11111111,	//3 !!
		0b11111000,0b00000011,0b11000011,0b11111111,	//4
		0b11111000,0b00111111,0b11111100,0b00011111,	//5
		0b11111111,0b11111111,0b11111100,0b00011111,	//6
		0b00000000,0b00111100,0b00000011,0b11111111,	//7
		0b11111111,0b11111111,0b11111111,0b11111111,	//8
		0b11111000,0b00111111,0b11111111,0b11111111,	//9
		0b11111111,0b11111111,0b11000011,0b11111111,	//A
		0b00000000,0b00000011,0b11000000,0b00000000,	//B
		0b11111111,0b11111111,0b11111111,0b11111111,	//C
		0b00000000,0b00000000,0b00000000,0b00000000,	//D
		};
uint8_t start_measure = 0;
uint8_t measure_ok = 0;
uint32_t measure_count = 0;
uint16_t time_div = 0;
uint8_t countdown_timer = 0;
uint8_t RGB_Index = 0; // Bit 2 = Blue -  Bit 1 = Green / Bit 0 = Red

void byte_out(int8_t data)
{
	register unsigned char bit=0;
	for(bit=0; bit<=7; bit++)
	{
		if( (data & 0x01) && (RGB_Index & 0x01)) { SDA_1(); } else { SDA_0(); }
		SCL_1();
		SCL_0();
		if( (data & 0x01) && (RGB_Index & 0x02)) { SDA_1(); } else { SDA_0(); }
		SCL_1();
		SCL_0();
		if( (data & 0x01) && (RGB_Index & 0x4)) { SDA_1(); } else { SDA_0(); }
		SCL_1();
		SCL_0();
		
		data = (data>>1);
	}
}
void do_rclk(void){
	RCL_1();
	RCL_0();
}
char load_font(uint8_t character){
	uint8_t offset = (character * 4)+3;
	for(uint8_t i=0; i<4; i++)
	{
		byte_out(flash_fonts[offset - i]);
	}
}

ISR(TIMER2_OVF_vect) {
	wdt_reset();
	
	if (start_measure && !measure_ok)
	{
		if ((PIND & (1 << PIND2)) != 0 && measure_count > 10)
		{
			measure_ok = 1;
		}else{
			measure_count++;
		}		
	}
	
	if (time_div && measure_ok)
	{
		time_div--;
	}else if(measure_ok){
		time_div= 18;
		
		if (countdown_timer == 0){
			countdown_timer = measure_count / 18;
		}else{
			countdown_timer--;
		}
	
		uint8_t A = countdown_timer/10;	
		load_font(countdown_timer - (A*10));
		load_font(A);
		do_rclk();
	}	
}

ISR(INT0_vect)
{
	if (!measure_ok && !start_measure)
	{
		measure_count = 0;
		start_measure = 1;
	}else{
	
	}
}



int main(void)
{
	wdt_enable(WDTO_8S);
	PORTA = 0B00000100;
	DDRA = 0b10011111;
	EIMSK |= _BV(INT0);
    EICRA |= _BV(ISC01);
	ASSR |= 1<<AS2;
	//TCCR2B = 0b00000101; //start timer for seconds
	TCCR2B = 0b00000010; //start timer for seconds
	TIMSK2 = 1;
	_delay_ms(100);
	
	RGB_Index = 0B00000111;
	load_font(11); //'-'
	load_font(11); //'-'
	do_rclk();
	DISPLAY_ON();
	_delay_ms(1000);
	RGB_Index = 1;
	sei();
    /* Replace with your application code */
    while (1) 
    {	
		_delay_ms(500);
		wdt_reset();
    }
}

