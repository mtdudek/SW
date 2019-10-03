////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 9, zadanie 2		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B |= _BV(TXEN0);
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

volatile float volt=0, sum=0.0, max=0.0;
volatile uint16_t counter=0;

void adc_init(){
  ADMUX  |= _BV(REFS0); 
  DIDR0   = _BV(ADC0D); 
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN) | _BV(ADIE); 
}

ISR(ADC_vect){
  float v = ADC-511.5; 
  volt=(float)v/511.5;
  sum+=volt*volt;
  if(volt<0) volt *=-1.0;
  if(max<volt) max=volt;
  counter++;
}

void timer0_init(){
  OCR0A=124;
  TCCR0A |= _BV(WGM01);
  TIMSK0 |= _BV(OCIE0A);
  TCCR0B |= _BV(CS01);
}

ISR(TIMER0_COMPA_vect){
  ADCSRA |= _BV(ADSC);
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit,NULL, _FDEV_SETUP_RW);
  stdout = stderr = &uart_file;
  sei();
  // inicjalizacja ADC
  adc_init();
  timer0_init();
  while(1) {
    _delay_ms(500);
    TIMSK0 &= ~_BV(OCIE0A);
    float sk=sqrt(sum/(float)counter);
    printf("%f\n\r",20.0*log10(sk/2.5*max));
    max=0;
    TIMSK0 |= _BV(OCIE0A);  
  }
}

