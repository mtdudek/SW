////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 1, zadanie 2		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define oneUnit 150			   // jedna jednostak w kodzie morsea

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

// kropka w kodzie morsa
void dot(){
    LED_PORT |= _BV(LED);
    _delay_ms(oneUnit);
    LED_PORT &= ~_BV(LED);
    _delay_ms(oneUnit);
}

// kreska w kodzie morsa
void dash(){
    LED_PORT |= _BV(LED);
    _delay_ms(3*oneUnit);
    LED_PORT &= ~_BV(LED);
    _delay_ms(oneUnit);
}

//slownik kodu morsa
char* t[]={
"-.-.--\0",".-..-.\0","\0","...-..-\0",
"\0",".-...\0",".----.\0","-.--.\0",
"-.--.-\0","\0",".-.-.\0","--..--\0",
"-....-\0",".-.-.-\0","-..-.\0","-----\0",
".----\0","..---\0","...--\0","....-\0",
".....\0","-....\0","--...\0","---..\0",
"----.\0","---...\0","-.-.-.\0","\0",
"-...-\0","\0","..--..\0",".--.-.\0",
".-\0","-...\0","-.-.\0","-..\0",
".\0","..-.\0","--.\0","....\0",
"..\0",".---\0","-.-\0",".-..\0",
"--\0","-.\0","---\0",".--.\0",
"--.-\0",".-.\0","...\0","-\0",
"..-\0","...-\0",".--\0","-..-\0",
"-.--\0","--..\0","\0","\0",
"\0","\0","..--.-\0"};

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // ustawienie diody
  LED_DDR |= _BV(LED);
  // program testowany
  char znak = 0,j = 0;
  while(1) {
    scanf("%c",&znak);
    j = znak-'!';
    if(znak-' '==0)
      _delay_ms(1000);
    else if(j>=0){
      if(j<63){
      	for(int i=0;t[j][i]!='\0';i++){
	  if(t[j][i]=='.') dot();
	  else dash();
	}
      }
      else if (0<=znak-'a'&&znak-'z'<=0){
      	for(int i=0;t[znak-'a'+32][i]!='\0';i++){
	  if(t[znak-'a'+32][i]=='.') dot();
	  else dash();
	}
      }
    }
  }
}

