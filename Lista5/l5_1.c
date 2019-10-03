////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 5, zadanie 1		////
////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED_DDR DDRD
#define LED_PORT PORTD
#define BUTTON_DDR DDRD
#define BUTTON_PORT PORTD

uint8_t tab[1000];
volatile uint16_t num=0;

void timer0_init(){
  TCCR0B |= _BV(CS01) | _BV(CS00);
  TIMSK0 |= _BV(OCIE0A);
  OCR0A = 124;
  TCCR0A |= _BV(WGM01);
}

ISR(TIMER0_COMPA_vect){
  //bufor cykliczny
  if(num>=1000)
    num-=1000;
  if(tab[num])
    LED_PORT |=0b00000100;
  else
    LED_PORT &=0b11111011;
  //czy naciśnięto guzik 
  if (!(PIND & 0b00010000) ){
    tab[num]=1;
  } else 
    tab[num]=0;
  num++;
}

int main(){
  //konfigurowanie portow
  LED_DDR &= 0b11111011;
  LED_PORT &= 0b11111011;
  BUTTON_DDR |= 0b00010000;
  BUTTON_PORT |= 0b00010000;
  // ustaw tryb uśpienia na tryb bezczynności
  set_sleep_mode(SLEEP_MODE_IDLE);
  // odmaskuj przerwania
  sei();
  // program testowany
  timer0_init();
  while(1) {
    sleep_mode();
  }
}
