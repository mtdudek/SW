////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 6, zadanie 2		////
////////////////////////////////////////////
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

#define Recive_Buf 100
char tab2[Recive_Buf];
volatile int8_t read2=-1,write2=0;

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

// odczyt jednego znaku
ISR(USART_RX_vect){
  if(write2>=Recive_Buf) write2-=Recive_Buf;
  tab2[write2]=UDR0;
  write2++;
}

int uart_receive(FILE *stream){
  while (read2+1==write2||(read2==Recive_Buf-1&&write2==0));
  read2++;
  if(read2>=Recive_Buf) read2-=Recive_Buf;
  return tab2[read2];
}

FILE uart_file;

/*
void my_debug(){
  //debug
  DDRD |= 0b11000000;
  uint16_t k=1;
  for (uint8_t i=0;i<8;i++){
    _delay_ms(1000);
    if(read1&(k<<i))
      PORTD |= 0b10000000;
    PORTD |= 0b01000000;
    _delay_ms(1000);
    PORTD &= 0b00111111;
  }
  _delay_ms(2000);
  for (uint8_t i=0;i<8;i++){
    _delay_ms(1000);
    if(write1&(k<<i))
      PORTD |= 0b10000000;
    PORTD |= 0b01000000;
    _delay_ms(1000);
    PORTD &= 0b00111111;
  }
}*/

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // maska przerwan
  sei();
  UCSR0B |= _BV(RXCIE0);
  printf("0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n\r");
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\n\r");
  printf("01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n\r");
  printf("Dziala wyjscie!!!!\n\rFORK YEA\n\r");
  char testtab[110];
  scanf("%s",&testtab);
  printf("%s\n\r",testtab);
  int8_t l1,l2;
  scanf("%"SCNd8"%"SCNd8,&l1,&l2);
  printf("%d %d\n\r",l1,l2);
  printf("Wejscie tez!!!");
  while(1) {
  }
}

