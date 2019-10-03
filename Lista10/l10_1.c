////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 10, zadanie 1	////
////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void adc_init(){
  ADMUX  |= _BV(REFS0); 
  DIDR0   = _BV(ADC0D); 
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); 
}

volatile uint16_t ADC_result=0;
volatile uint16_t start_up=3;

ISR(ADC_vect){
  ADC_result = 1023 - ADC; 
  if(ADC_result >= 1010)
    start_up=1000;
  if(start_up&&ADC_result<950){
    OCR1A=768;
    start_up--;
  } else if(ADC_result<950)
    OCR1A=ADC_result; 
  else
    OCR1A=1024;
}

void timer1_init(){
  TCCR1A |= _BV(COM1A1) | _BV(COM1A0);
  TCCR1B |= _BV(WGM13) | _BV(CS11);
  ICR1=1024;
  DDRB |= _BV(PB1);
}

int main(){
  sei();
  // inicjalizacja ADC
  adc_init();
  timer1_init();
  ADCSRA |= _BV(ADSC);
  while(1) { 
  }
}

