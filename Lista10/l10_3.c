////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 10, zadanie 3	////
////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_init(){
  ADMUX  |= _BV(REFS0); 
  DIDR0   = _BV(ADC0D); 
  ADCSRA |= _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN); 
}

void timer1_init(){
  TCCR1A |= _BV(COM1A1) | _BV(WGM11) | _BV(COM1B1);
  TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11);
  ICR1=3072;
  DDRB |= _BV(PB1) | _BV(PB2);
}

int main(){
  adc_init();
  timer1_init();
  uint16_t ADC_result=0;
  while(1) { 
    ADCSRA |= _BV(ADSC);
    while (!(ADCSRA & _BV(ADIF))); 
    ADCSRA |= _BV(ADIF);
    ADC_result = ADC; 
    if(ADC_result>512){
      OCR1A=(ADC_result-512)*6;
      OCR1B=0;
    }
    else{
      OCR1A=0;
      OCR1B=(512-ADC_result)*6;
    }
  }
}

