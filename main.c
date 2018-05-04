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

int main(void) {
	cli();
	ping_setTimer();
	//DDRB=(1 << PB7);
	DDRB=(1<<PB5); //PA7 (29)
	init_USART();
	sei();
	while(1){
		test_Servo();
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

void test_Servo(void)
{
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);	
	ICR1 = 24999;		
	while(1){
		OCR1A = 4900;

		_delay_ms(2000);

		OCR1A = 1500;

		_delay_ms(2000);
	}
}
