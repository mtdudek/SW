////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 4, zadanie 1		////
////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

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

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  TCCR1B = _BV(CS10);
  volatile uint8_t ra,a;
  volatile uint16_t rb,b;
  volatile uint32_t rc,c;
  volatile uint64_t rd,d;
  volatile double rf,f;
  TCNT1=0; ra=a+a; uint16_t a1= TCNT1;
  TCNT1=0; ra=a-a; uint16_t a2= TCNT1;
  TCNT1=0; ra=a*a; uint16_t a3= TCNT1;
  TCNT1=0; ra=a/a; uint16_t a4= TCNT1;
  printf("Zmierzony czas: %"PRIu16",%"PRIu16",%"PRIu16",%"PRIu16", cykli\r\n",a1,a2,a3,a4);
  TCNT1=0; rb=b+b; uint16_t b1= TCNT1;
  TCNT1=0; rb=b-b; uint16_t b2= TCNT1;
  TCNT1=0; rb=b*b; uint16_t b3= TCNT1;
  TCNT1=0; rb=b/b; uint16_t b4= TCNT1;
  printf("Zmierzony czas: %"PRIu16",%"PRIu16",%"PRIu16",%"PRIu16", cykli\r\n",b1,b2,b3,b4);
  TCNT1=0; rc=c+c; uint16_t c1= TCNT1;
  TCNT1=0; rc=c-c; uint16_t c2= TCNT1;
  TCNT1=0; rc=c*c; uint16_t c3= TCNT1;
  TCNT1=0; rc=c/c; uint16_t c4= TCNT1;
  printf("Zmierzony czas: %"PRIu16",%"PRIu16",%"PRIu16",%"PRIu16", cykli\r\n",c1,c2,c3,c4);
  TCNT1=0; rd=d+d; uint16_t d1= TCNT1;
  TCNT1=0; rd=d-d; uint16_t d2= TCNT1;
  TCNT1=0; rd=d*d; uint16_t d3= TCNT1;
  TCNT1=0; rd=d/d; uint16_t d4= TCNT1;
  printf("Zmierzony czas: %"PRIu16",%"PRIu16",%"PRIu16",%"PRIu16", cykli\r\n",d1,d2,d3,d4);
  TCNT1=0; rf=f+f; uint16_t f1= TCNT1;
  TCNT1=0; rf=f-f; uint16_t f2= TCNT1;
  TCNT1=0; rf=f*f; uint16_t f3= TCNT1;
  TCNT1=0; rf=f/f; uint16_t f4= TCNT1;
  printf("Zmierzony czas: %"PRIu16",%"PRIu16",%"PRIu16",%"PRIu16", cykli\r\n",f1,f2,f3,f4);
 
  while(1) {
  }
}

