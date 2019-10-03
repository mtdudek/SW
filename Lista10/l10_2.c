////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 10, zadanie 2	////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600                          
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

// inicjalizacja UART
void uart_init(){
  UBRR0 = UBRR_VALUE;
  UCSR0B |= _BV(TXEN0);
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

void adc_init(){
  ADMUX  |= _BV(REFS0); 
  DIDR0   = _BV(ADC0D) | _BV(ADC1D); 
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN); 
}

void timer1_init(){
  TCCR1A |= _BV(COM1A1) | _BV(COM1A0);
  TCCR1B |= _BV(WGM13) | _BV(CS11);
  ICR1=1024;
  DDRB |= _BV(PB1);
}

ISR(TIMER1_CAPT_vect){
    TIMSK1 &= ~_BV(ICIE1);
    ADCSRA |= _BV(ADSC);
}

ISR(TIMER1_OVF_vect){
    ADCSRA |= _BV(ADSC);
    TIMSK1 &= ~_BV(TOIE1);
}

int main(){
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit,NULL, _FDEV_SETUP_RW);
  stdout = stderr = &uart_file;
  sei();
  adc_init();
  timer1_init();
  uint16_t ADC_result=0;
  uint8_t start_up=1,counter=0;
  uint16_t ADC_lower,ADC_upper;
  while(1) { 
    ADMUX &= ~_BV(MUX0);
    ADCSRA |= _BV(ADSC);
    while (!(ADCSRA & _BV(ADIF))); 
    ADCSRA |= _BV(ADIF);
    ADC_result = 1023 - ADC; 
    if(ADC_result >= 1010)
      start_up=1;
    if(start_up&&ADC_result<975){
      OCR1A=768;
      start_up--;
    } else if(ADC_result<975)
      OCR1A=ADC_result; 
    else
      OCR1A=1024;
    if(counter==100){
      ADMUX |= _BV(MUX0); 
      counter=0;
      TIMSK1 |= _BV(ICIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_upper=ADC;

      TIMSK1 |= _BV(TOIE1);
      while (!(ADCSRA & _BV(ADIF))); 
      ADCSRA |= _BV(ADIF);
      ADC_lower=ADC;

      float f1=(float)ADC_upper*5.0/1024.0,
            f2=(float)ADC_lower*5.0/1024.0;
      printf("Napięcia H=%.4fmV,L=%.4fmV\r",f1,f2);
    }
    counter++;
    _delay_ms(10);
  }
}

