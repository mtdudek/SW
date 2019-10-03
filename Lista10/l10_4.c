////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 10, zadanie 4	////
////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init(){
  ADMUX  |= _BV(REFS0); 
  DIDR0  |= _BV(ADC0D); 
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); 
}

volatile uint16_t ADC_result=0;

ISR(ADC_vect){
  ADC_result = 1023-ADC; 
  OCR1A=125+ADC_result;
}

void timer1_init(){
  TCCR1A |= _BV(COM1A1) | _BV(WGM11);
  TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
  ICR1=4999;
  DDRB |= _BV(PB1);
}

int main(){
  sei();
  adc_init();
  timer1_init();
  ADCSRA |= _BV(ADSC);
  while(1) { 
    printf("%lu\n\r",OCR1A);
  }
}

