#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

typedef unsigned char byte;

#define BAUDRATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void wait(unsigned);
void init_USART();
void USART_send(byte data);
char USART_receive();
void USART_sendString(char st[]);
unsigned char get_frequency();

int main() {

	DDRB=(1 << PB7);
	init_USART();
	
	unsigned int hertz = (10/(unsigned int)get_frequency());
	
	while(1) {
		PORTB ^=(1 << PB7);
		//USART_sendString("test");
		wait(hertz);
	}
}

unsigned char get_frequency()
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
			temp = '10';
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

void init_USART()
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

char USART_receive()
{
	while (! (UCSR0A & (1<<RXC0)));
	return UDR0;
}