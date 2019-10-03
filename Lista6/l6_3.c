////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 6, zadanie 3		////
////////////////////////////////////////////
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include "sound.h"

void spi_init(){
    // ustaw piny MOSI, SCK i ~SS jako wyjścia
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    // włącz SPI w trybie master z zegarem 250 kHz
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
}

// transfer jednego bajtu
uint8_t spi_transfer(uint8_t data){
    // rozpocznij transmisję
    SPDR = data;
    // czekaj na ukończenie transmisji
    while (!(SPSR & _BV(SPIF)));
    // wyczyść flagę przerwania
    SPSR |= _BV(SPIF);
    // zwróć otrzymane dane
    return 0;
}

//Timer 0 init
void timer0_init(){
  OCR0A=124;
  TCCR0A |= _BV(WGM01);
  TIMSK0 |= _BV(OCIE0A);
  TCCR0B |= _BV(CS01);
  PORTB ^= _BV(DDB1);
}

volatile uint16_t i=0;

ISR(TIMER0_COMPA_vect){
  if(i>=out_raw_len) i=0;
  uint8_t data=pgm_read_word(&out_raw[i]);
  i++;
  PORTB &= ~_BV(DDB2);
  spi_transfer(0b01110000|(data>>4));
  spi_transfer(data<<4);
  PORTB |= _BV(DDB2);
}

int main(){
  // zainicjalizuj SPI
  sei();
  spi_init();
  DDRB |= _BV(DDB1);
  PORTB |= _BV(DDB2);
  timer0_init();
  while(1) {
  }
}
