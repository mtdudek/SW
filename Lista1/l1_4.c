////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 1, zadanie 4		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD

void set_9(){
  LEDs_PORT ^=0b11101111;
}

int main()
{
  //konfigurowanie portow
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  LEDs_DDR |= 0b11111111;
  LEDs_PORT |= 0b11111111;
  // program testowany
  set_9();
  while(1) {
    _delay_ms(1000);
    LEDs_PORT ^=0b10010000;
    _delay_ms(1000);
    LEDs_PORT ^=0b01111000;
    _delay_ms(1000);
    LEDs_PORT ^=0b11111010;
    _delay_ms(1000);
    LEDs_PORT ^=0b10010000;
    _delay_ms(1000);
    LEDs_PORT ^=0b00001011;
    _delay_ms(1000);
    LEDs_PORT ^=0b00101001;
    _delay_ms(1000);
    LEDs_PORT ^=0b00010100;
    _delay_ms(1000);
    LEDs_PORT ^=0b01011101;
    _delay_ms(1000);
    LEDs_PORT ^=0b10111001;
    _delay_ms(1000);
    LEDs_PORT ^=0b01010000;
  }
}

