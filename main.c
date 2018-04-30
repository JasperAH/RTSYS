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
unsigned char USART_receive();
void USART_sendString(char st[]);

int main() {

	DDRB=(1 << PB7);
	init_USART();
	
	unsigned int hertz = (10/(unsigned int)USART_receive());
	
	while(1) {
		PORTB ^=(1 << PB7);
		//USART_sendString("test");
		wait(hertz);
	}
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

unsigned char USART_receive()
{
	while (! (UCSR0A & (1<<RXC0)));
	unsigned char temp;
	char debug[30];
	switch(UDR0)
	{
		case 1: 
			temp = '1'; 
			strcpy(debug, "Gekozen voor 1"); 
			break;
		case 2: 
			temp = '2'; 
			strcpy(debug, "Gekozen voor 2"); 
			break;
		case 3: 
			temp = '10'; 
			strcpy(debug, "Gekozen voor 10"); 
			break; 
	} 
	USART_sendString(debug);	
	return temp;
}