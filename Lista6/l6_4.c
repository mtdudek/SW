////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 6, zadanie 4		////
////////////////////////////////////////////
#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

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

int8_t num_bit_rep[]={
0b11000000,
0b11111001,
0b10100100,
0b10110000,
0b10011001,
0b10010010,
0b00000010,
0b11111000,
0b10000000,
0b00010000};


int main(){
  // zainicjalizuj SPI
  spi_init();
  DDRB |= _BV(DDB1);
  uint8_t i=0;
  PORTB |= _BV(DDB1);
  while(1) {
    if(i>=10) i-=10;
    PORTB &= ~_BV(DDB1);
    spi_transfer(~num_bit_rep[i]);
    PORTB |= _BV(DDB1);
    i++;
    _delay_ms(1000);
  }
}
