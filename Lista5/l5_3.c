////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 5, zadanie 3		////
////////////////////////////////////////////
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

volatile float tab1[128],tab2[128];

// inicjalizacja ADC
void adc_init(){
  ADMUX   = 0b01001110;
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

uint8_t i2=0;

ISR(ADC_vect){
  uint16_t v = ADC;
  float g = (float)v/(float)1023;
  float a=1.1/g;
  i2++;
  tab2[i2]=a;
}

float pot(float h){
 return h*h;
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
 
  adc_init();
  for (int i=0;i<40;i++){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  }
  for (int i=0;i<128;i++){
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną i zmień do zbioru (0..1023)
    float g = (float)v/(float)1023;
    float a=1.1/g;
    tab1[i]=a;
  }
  ADCSRA |= _BV(ADIE);
  sei();
  set_sleep_mode(SLEEP_MODE_IDLE);
  for (int i=0;i<128;i++){
    sleep_mode();
  }
  cli(); 
  volatile float s1=0,s2=0,w1=0,w2=0;
  for (int i=0;i<128;i++){
    s1+=tab1[i];s2+=tab2[i];
  }
  s1/=128.0;s2/=128.0;
  for (int i=0;i<128;i++){
    w1+=pot(s1-tab1[i]);w2+=pot(s2-tab2[i]);
  }
  printf("%f %f\r\n",w1/128,w2/128);
  while(1) {
    _delay_ms(100);
  }
}
