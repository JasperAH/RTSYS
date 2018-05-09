#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

typedef unsigned char byte;

#define BAUDRATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

uint16_t msCounter = 0;
uint8_t ppmPosition = 0; // -90 bij 9, 0 bij 15, 90 bij 21


int main(void) {
	cli();
	ping_setTimer();
	//DDRB=(1 << PB7);
	DDRA=(1<<PA7); //PA7 (29)
	init_USART();
	initServo();
	sei();
	while(1){
		wait(50);
		ppmPosition = 9;
		wait(50);
		ppmPosition = 21;
		/*
		ping_getDistance();
		if(result != 0){
			USART_sendString("Distance: ");
			char buf[8];
			itoa(result, buf, 10);
			USART_sendString(buf);
			USART_send('\n');
			_delay_ms(500);
			result = 0;
		}*/
	}
	//unsigned int hertz = (10/(get_frequency()-'0'));
	
	/*
	while(1) {
		PORTB ^=(1 << PB7);
		//USART_sendString("test");
		wait(hertz);
	}
	*/
	
	
}

unsigned char get_frequency(void)
{
	unsigned char temp;
	char receive;
	
	receive = USART_receive();
	
	switch(receive)
	{
		case '1':
			temp = '1';
			USART_sendString("Gekozen voor 1");
			break;
		case '2':
			temp = '2';
			USART_sendString("Gekozen voor 2");
			break;
		case '3':
			temp = ':'; //getal 10
			USART_sendString("Gekozen voor 10");
			break;
		default:
			USART_sendString("Onbekende input");
			temp = '0';
	}
	return temp;
}

void wait(unsigned a) {
	while(a--)
	_delay_ms(50);
}

void init_USART(void)
{
	UBRR0 = BAUD_PRESCALER;
	
	UCSR0C = ((0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00));
	
	UCSR0B = ((1 << RXEN0) | (1 << TXEN0));
}

void USART_send(byte data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

void USART_sendString(char st[])
{
	for(int i = 0; st[i] != 0; i++)
	{
		USART_send(st[i]);
	}
}

char USART_receive(void)
{
	while (! (UCSR0A & (1<<RXC0)));
	return UDR0;
}

void ping_getDistance(void)
{
	//PB0 (53) PB2 (51)
	DDRB |= (1<<PB0);
	PORTB &= ~(1<<PB0);
	_delay_us(2);
	PORTB |= (1<<PB0);
	_delay_us(5);
	PORTB &= ~(1<<PB0);
	DDRB &= ~(1<<PB2);
	ping_listen();
}

void ping_listen(void)
{
		TCNT1 = 0;
		while(~PINB & 1<<PB2);
		//if(TCNT1 > 500000) break;
		TCNT1 = 0;								//Reset timer naar 0
		while(PINB & 1<<PB2);
		//if(TCNT1 > 500000) break;
		timerValue = TCNT1;						//Neem TCNT1 waarde over en stop deze in INT waarde TimerValue
		result = (timerValue /2 /29 /2);
}
void ping_setTimer(void){
	TCCR1B |= (1<<CS11);					//prescaler is set 8
}

void initServo(void)
{
	TCCR3A |= (0 << WGM31) | (1 << COM3A1);
	TCCR3B |= (1 << WGM32) | (0 << WGM33) | (1 << CS30);	
	TCNT3 = 0;
	OCR3A = 1600;		
	TIMSK3 |= (1<<OCIE3A);
}



ISR(TIMER3_COMPA_vect)
{
	if(msCounter < ppmPosition){
		PORTA |= (1<<PA7);
	} else {
		PORTA &= ~(1<<PA7);
	}
	
	if(msCounter < 200){
		msCounter++;
	} else {
		msCounter = 0;
	}
}