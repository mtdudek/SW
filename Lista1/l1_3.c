////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 1, zadanie 3		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD

int main()
{
  //konfigurowanie portow
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  LEDs_DDR |= 0b11111111;
  LEDs_PORT |= 0b01000000;
  // program testowany
  while(1) {
    for(int i=0;i<6;i++){
      LEDs_PORT >>=1;
      _delay_ms(100);
    }
    for(int i=0;i<6;i++){
      LEDs_PORT <<=1;
      _delay_ms(100);
    }
  }
}

