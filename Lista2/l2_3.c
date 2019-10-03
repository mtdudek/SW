////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 2, zadanie 3		////
////////////////////////////////////////////
#include <util/delay.h>
#include <avr/io.h>

#define LEDs_DDR DDRD
#define LEDs_PORT PORTD
#define BUTTONs_DDR DDRC
#define BUTTONs_PORT PORTC

void convert_to_gray_and_set(uint8_t num){
  uint8_t temp = num ^ (num >> 1);
  LEDs_PORT = temp;
}

int main()
{//konfigurowanie portow
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  LEDs_DDR |= 0b11111111;
  LEDs_PORT &= 0b00000000;
  BUTTONs_DDR &= 0b11111000;
  BUTTONs_PORT |= 0b00000111;
  // program testowany
  uint8_t pressed1 = 0, pressed2 = 0, pressed3 = 0, num = 0;
  while(1) {
    //czy naciśnięto guzik kolejnego kodu Gray-a
    if (!(PINC & 0b00000001) && !pressed1){
      num ++;
      pressed1 = 1;
    } else if((PINC & 0b00000001) && pressed1)
      pressed1 = 0;
    //czy naciśnięto guzik poprzedniego kodu Gray-a
    if (!(PINC & 0b00000010) && !pressed2){
      num --;
      pressed2 = 1;
    } else if((PINC & 0b00000010) && pressed2)
      pressed2 = 0;
    //czy naciśnięto guzik resetowanie kodu Gray-a
    if (!(PINC & 0b00000100) && !pressed3){
      num = 0;
      pressed3 = 1;
    } else if((PINC & 0b00000100) && pressed3)
      pressed3 = 0;
    //konwertowanie liczby binarnej do kodu Gray-a
    convert_to_gray_and_set(num);
    _delay_ms(10);
  }
}

