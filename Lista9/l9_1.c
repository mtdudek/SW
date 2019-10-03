////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 9, zadanie 1		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz nadajnik
  UCSR0B = _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
ISR(USART_UDRE_vect){
  if( (read1+1==Transmit_Buf&&write1==0) ||read1+1==write1){
    UCSR0B &= ~_BV(UDRIE0);
    return;
  }
  read1++;
  if(read1>=Transmit_Buf) read1-=Transmit_Buf;
  UDR0=tab1[read1];
}

int uart_transmit(char data, FILE *stream){
  while ((write1==read1)||(write1==Transmit_Buf&&read1<1));
  if (write1>=Transmit_Buf) write1-=Transmit_Buf;
  tab1[write1]=data;
  write1++;
  if ((read1==99&&write1==1)||read1+2 == write1) 
    UCSR0B |= _BV(UDRIE0);
  return 0;
}

FILE uart_file;

void adc_init(){
  ADMUX   = _BV(REFS1) | _BV(REFS0); 
  DIDR0   = _BV(ADC0D); 
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN); 
}

static float B=3073.0, T0=291, R0=5500.0;

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, NULL, _FDEV_SETUP_RW);
  stdout = stderr = &uart_file;
  DDRB |= _BV(PB5);
  // inicjalizacja ADC
  adc_init();
  for (int i=0;i<40;i++){
    ADCSRA |= _BV(ADSC); 
    while (!(ADCSRA & _BV(ADIF))); 
    ADCSRA |= _BV(ADIF); 
  }
  sei();
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną i zmień do zbioru (0..1023)
    float na = 1.1*(float)v/(float)1024;
    float r = na*1000000/145; //taki jest prąd generowany przez moje źródło
    printf("%f C\n\r",B*T0/(log(r/R0)*T0+B)-273);
    _delay_ms(1000);
  }
}

