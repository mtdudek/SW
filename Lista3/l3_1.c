////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 3, zadanie 1		////
////////////////////////////////////////////
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

//definowanie tonów
#define c 130
#define d 147
#define e 165
#define f 175
#define g 196
#define a 220
#define b 247
#define c1 262
#define d1 294
#define e1 330
#define f1 349
#define g1 392
#define a1 440
#define b1 494
#define c2 523
#define d2 587
#define e2 659
#define f2 698
#define g2 784
#define a2 880
#define b2 988
#define c3 1046
//*************

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define TONE(step, delay) \
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      _delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      _delay_us(step); \
    }

const int tempo=125;

#include<stdio.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

static const uint16_t utwor[] PROGMEM = {
   c1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, c2, d2, d2, 0, 
   d2, e2, f2, f2, 0,
   e2, d2, e2, a1, 0,
   a1, b1, c2, c2, 0, 
   d2, e2, a1, 0, 
   a1, c2, b1, b1, 0,
   c2, a1, b1, 0,
   a1, a1, 
   //Repeat of first part
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, c2, d2, d2, 0, 
   d2, e2, f2, f2, 0,
   e2, d2, e2, a1, 0,
   a1, b1, c2, c2, 0, 
   d1, e2, a1, 0, 
   a1, c2, b1, b1, 0,
   c2, a1, b1, 0,
   //End of Repeat
   e2, 0, 0, f2, 0, 0,
   e2, e2, 0, g2, 0, e2, d2, 0, 0,
   d2, 0, 0, c2, 0, 0,
   b1, c2, 0, b1, 0, a1,
   e2, 0, 0, f2, 0, 0,
   e2, e2, 0, g2, 0, e2, d2, 0, 0,
   d2, 0, 0, c2, 0, 0,
   b1, c2, 0, b1, 0, a1
};

static const int duration[] PROGMEM = {         
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 1, 2, 1,
  1, 1, 2, 1, 1, 
  2, 1, 2, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 2,
  2, 1,
  //Rpeat of First Part
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1,   
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 1, 2, 1,
  1, 1, 2, 1, 1, 
  2, 1, 2, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 2,
  //End of Repeat
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 2,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 4,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 2,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 4
};


int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  BUZZ_DDR |= _BV(BUZZ); 
  uint16_t j;
  while(1) {
    for (int i=0;i<203;i++){  
      printf("%d\n\r",i); 
      uint16_t 	delay = pgm_read_word(&duration[i])*tempo,
		step = pgm_read_word(&utwor[i]);
      if(step==0)
	for(int i=0;i<delay;i++)
	_delay_ms(1);
      else{
      for (uint16_t i = 0; i < (uint32_t)(1000)*delay/step/2; i++) {
        BUZZ_PORT |= _BV(BUZZ);
	j=step;
	while(j--)
      	  _delay_us(1); 
        BUZZ_PORT &= ~_BV(BUZZ);
	j=step;
	while(j--)
      	  _delay_us(1); 
      }}
    }          
  }
}

