////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 2, zadanie 2		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init(){
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
int uart_transmit(char data, FILE *stream){
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream){
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

char morse[40] = {' ', ' ','e','t','i','a','n','m','s','u','r','w','d','k','g','o','h','v','f',' ','l',' ','p','j','b','x','c','y','z','q',' ',' '} ;

int i = 1;

int main(){ 
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  //konfigurowanie portow
  LEDs_DDR |= 0b11100000;
  LEDs_PORT &= 0b00000000;
  BUTTON_DDR &= 0b11111110;
  BUTTON_PORT |= 0b00000001;
  // program testowany
  uint8_t pressed = 0,notpressed = 0;
  while(1) {
    //czy naciśnięto guzik 
    if (!(PINC & 0b00000001) ){
      notpressed = 0;
      pressed++;
    } else {
      notpressed++;
      if(pressed<3&&pressed>0){
  	LEDs_PORT |= 0b10000000;
	i*=2;
    	_delay_ms(75);
	LEDs_PORT &= 0b00000000;
      } else if (pressed>=3){
  	LEDs_PORT |= 0b01000000;
	i=i*2+1;
    	_delay_ms(75);
	LEDs_PORT &= 0b00000000;
      }
      pressed=0;
    }   
    if (notpressed == 5){
	printf("%c",morse[i]);
  	LEDs_PORT |= 0b00100000;
	i=1;
    	_delay_ms(50);
	LEDs_PORT &= 0b00000000;
    }
    _delay_ms(100);
  }
}

