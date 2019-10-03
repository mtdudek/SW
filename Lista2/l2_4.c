////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 2, zadanie 4		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD
#define LEDs_POWER_DDR DDRC
#define LEDs_POWER_PORT PORTC

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

void refresh(int8_t num, int8_t bit){
  LEDs_PORT = num_bit_rep[num];
  LEDs_POWER_PORT &= ~(1<<bit);
  _delay_ms(5);
  LEDs_PORT = 0b11111111;
  LEDs_POWER_PORT |= (1<<bit);
}

int main()
{
  //konfigurowanie portow
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  LEDs_DDR |= 0b11111111;
  LEDs_PORT |= 0b11111111;
  LEDs_POWER_DDR |= 0b00000011;
  LEDs_POWER_PORT |= 0b00000011;
  int8_t c=-1,j=0,d=0;
  // program testowany
  while(1) {
    c++;
    c -= (c/60)*60;
    d = c/10;
    j = c -d*10;
    for(int i=0;i<100;i++){
      if(d > 0)
        refresh(d,0);
      else
        _delay_ms(5);
      refresh(j,1);
    }
  }
}

