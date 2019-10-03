////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 9, zadanie 3		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz nadajnik
  UCSR0B = _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
ISR(USART_UDRE_vect){
  if( (read1+1==Transmit_Buf&&write1==0) ||read1+1==write1){
    UCSR0B &= ~_BV(UDRIE0);
    return;
  }
  read1++;
  if(read1>=Transmit_Buf) read1-=Transmit_Buf;
  UDR0=tab1[read1];
}

int uart_transmit(char data, FILE *stream){
  while ((write1==read1)||(write1==Transmit_Buf&&read1<1));
  if (write1>=Transmit_Buf) write1-=Transmit_Buf;
  tab1[write1]=data;
  write1++;
  if ((read1==99&&write1==1)||read1+2 == write1) 
    UCSR0B |= _BV(UDRIE0);
  return 0;
}

FILE uart_file;

volatile uint16_t last=0,delta;
volatile float freq=0.0;

void timer1_init(){
  // ustaw tryb licznika
  OCR1A = 31249;
  TCCR1B |= _BV(WGM12) | _BV(ICES1) | _BV(CS12);
  TIMSK1 |= _BV(ICIE1);
  // uastaw port ICP1 na wejście
  DDRB &= ~_BV(PB0);
}

//obliczanie częstotliwości
ISR(TIMER1_CAPT_vect){
  delta=ICR1-last;
  last=ICR1;
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, NULL, _FDEV_SETUP_RW);
  stdout = stderr = &uart_file;
  timer1_init();
  sei();
  while(1) {
    freq=31250.0/(float)delta;
    printf("%f\n\r",freq);
    _delay_ms(1000);
  }
}

