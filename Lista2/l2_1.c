////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 2, zadanie 1		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC

uint8_t tab[1000];

int main()
{//konfigurowanie portow
  LEDs_DDR |= 0b11100000;
  LEDs_PORT &= 0b00000000;
  BUTTON_DDR &= 0b11111110;
  BUTTON_PORT |= 0b00000001;
  // program testowany
  uint8_t num = 0;
  while(1) {
    if(num>=1000)
      num-=1000;
    if(tab[num])
      LEDs_PORT |=0b10000000;
    else
      LEDs_PORT &=0b01111111;
    //czy naciśnięto guzik 
    if (!(PINC & 0b00000001) ){
      tab[num]=1;
    } else 
      tab[num]=0;
    num++;
    _delay_ms(1);
  }
}

