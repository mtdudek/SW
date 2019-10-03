////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 3, zadanie 3		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

int main()
{	
  DDRD |= _BV(PD2);
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie 
  uint16_t p = 1;
  p<<=15;
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    uint16_t g=v/64; 
    uint16_t k=p/((uint16_t)1<<g);
    k--;
    for (uint16_t i=0;i < p;i++){
      if(i&k)
      	PORTD &= ~_BV(PD2);
      else
	PORTD |= _BV(PD2);
    }
  }
}

