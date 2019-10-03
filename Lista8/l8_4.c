////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 8, zadanie 4		////
////////////////////////////////////////////
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

//definowanie tonów
#define c 240
#define d 213
#define e 189
#define f 179
#define g 159
#define a 142
#define b 127
#define c1 119
#define d1 106
#define e1 95
#define f1 90
#define g1 80
#define a1 71
#define b1 63
#define c2 60
#define d2 53
#define e2 47
#define f2 45
#define g2 40
#define a2 36
#define b2 32
#define c3 30
//*************

void timer1_init(){
  TCCR1A |= _BV(COM1A0) | _BV(WGM11) | _BV(WGM10);
  TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS12);
}

const int tempo=125;

static const uint16_t utwor[] PROGMEM = {
   c1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, c2, d2, d2, 0, 
   d2, e2, f2, f2, 0,
   e2, d2, e2, a1, 0,
   a1, b1, c2, c2, 0, 
   d2, e2, a1, 0, 
   a1, c2, b1, b1, 0,
   c2, a1, b1, 0,
   a1, a1, 
   //Repeat of first part
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, b1, c2, c2, 0, 
   c2, d2, b1, b1, 0,
   a1, g1, a1, 0,
   e1, g1, a1, a1, 0, 
   a1, c2, d2, d2, 0, 
   d2, e2, f2, f2, 0,
   e2, d2, e2, a1, 0,
   a1, b1, c2, c2, 0, 
   d1, e2, a1, 0, 
   a1, c2, b1, b1, 0,
   c2, a1, b1, 0,
   //End of Repeat
   e2, 0, 0, f2, 0, 0,
   e2, e2, 0, g2, 0, e2, d2, 0, 0,
   d2, 0, 0, c2, 0, 0,
   b1, c2, 0, b1, 0, a1,
   e2, 0, 0, f2, 0, 0,
   e2, e2, 0, g2, 0, e2, d2, 0, 0,
   d2, 0, 0, c2, 0, 0,
   b1, c2, 0, b1, 0, a1
};

static const int duration[] PROGMEM = {         
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 1, 2, 1,
  1, 1, 2, 1, 1, 
  2, 1, 2, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 2,
  2, 1,
  //Rpeat of First Part
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 2, 1,   
  1, 1, 2, 1, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  1, 1, 1, 2, 1,
  1, 1, 2, 1, 1, 
  2, 1, 2, 1, 
  1, 1, 2, 1, 1,
  1, 1, 2, 2,
  //End of Repeat
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 2,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 4,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 2,
  2, 1, 2, 2, 1, 2,
  1, 1, 1, 1, 1, 4
};

int main(){
  uint16_t j;
  timer1_init();
  while(1) {
    for (int i=0;i<203;i++){  
      uint16_t 	delay = pgm_read_word(&duration[i])*tempo,
		step = pgm_read_word(&utwor[i]);
      if(step==0){
  	DDRB &= ~_BV(PB1); 
	for(int i=0;i<delay;i++)
	  _delay_ms(1);
      }
      else{
  	DDRB |= _BV(PB1);
        OCR1A=step;
	for(int i=0;i<delay;i++)
	  _delay_ms(1);
      }
    }          
  }
}
