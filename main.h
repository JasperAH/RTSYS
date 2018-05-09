/*
 * main.h
 *
 * Created: 4-5-2018 11:06:07
 *  Author: dionr
 */ 


#ifndef MAIN_H_
#define MAIN_H_

volatile uint16_t timerValue;
uint16_t result = 0;

void wait(unsigned);
void init_USART(void);
void USART_send(byte data);
char USART_receive(void);
void USART_sendString(char st[]);
unsigned char get_frequency(void);
void ping_getDistance(void);
void ping_listen(void);
void ping_setTimer(void);
void initServo(void);

#endif /* MAIN_H_ */