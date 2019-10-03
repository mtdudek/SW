////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 8, zadanie 1		////
////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"
#include <avr/interrupt.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

volatile uint8_t pos=0;

volatile char oldbuf[17]={"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};

#define Recive_Buf 100
char tab2[Recive_Buf];
volatile int8_t read2=-1,write2=0;

// inicjalizacja UART
void uart_init(){
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// odczyt jednego znaku
ISR(USART_RX_vect){
  if(write2>=Recive_Buf) write2-=Recive_Buf;
  UDR0=tab2[write2]=UDR0;
  write2++;
}

int uart_receive(FILE *stream){
  while (read2+1==write2||(read2==Recive_Buf-1&&write2==0));
  read2++;
  if(read2>=Recive_Buf) read2-=Recive_Buf;
  return tab2[read2];
}

FILE uart_file;

int hd44780_transmit(char data, FILE *stream){
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

int main(){
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;
  // zainicjalizuj UART
  uart_init();
  fdev_setup_stream(&uart_file, NULL, uart_receive, _FDEV_SETUP_READ);
  stdin = &uart_file;
  // program testowy
  LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
  printf("Hello world!");
  LCD_GoTo(0, 1);
  sei();
  UCSR0B |= _BV(RXCIE0);
  char buff,*a;
  while(1) {
    do{
      a=fgets(&buff,2,stdin);
    }while(a!=NULL&& a!=&buff);
    if(buff!=10){
      LCD_GoTo(pos,1);
      printf("%c",buff);
      oldbuf[pos]=buff;
      pos++;
    }
    if(buff==10||pos==16){
      pos=0;
      LCD_Clear();
      LCD_GoTo(pos,0);
      printf("%s",oldbuf);
      for(uint8_t i=0;i<16;i++)  oldbuf[i]=0;
    }
    buff=0;
  }
}

