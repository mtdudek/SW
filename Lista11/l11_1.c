////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 11, zadanie 2	////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include "pid.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define Transmit_Buf 100
char tab1[Transmit_Buf];
volatile int8_t read1=-1,write1=0;

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

void adc_init(){
  ADMUX   = _BV(REFS1) | _BV(REFS0); 
  DIDR0   = _BV(ADC0D); 
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); 
  ADCSRA |= _BV(ADEN) | _BV(ADIE); 
}

volatile uint16_t adc_expected,
		  adc_result;

int16_t res; 
int16_t P=30;

struct PID_DATA pidData;

void timer0_init(){
  OCR0A=255;
  TCCR0A |= _BV(WGM01);
  TIMSK0 |= _BV(OCIE0A);
  TCCR0B |= _BV(CS02) | _BV(CS00);
}

void timer2_init(){  
  DDRB |= _BV(PB3);
  TCCR2A |= _BV(WGM01) | _BV(COM2A1) | _BV(WGM20) | _BV(WGM21);
  TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
}

ISR(ADC_vect){
  adc_result=ADC;
}

ISR(TIMER0_COMPA_vect){
  res=pid_Controller(adc_expected,adc_result,&pidData)/128;
  float temp=(adc_result*1.1/1024.0-0.5)/0.01;
  printf("%.4d %.3fC\r\n",res,temp);
  if(res>255)
    OCR2A=255;
  else if(res>=0)
    OCR2A=res;
  else
    OCR2A=0;
  ADCSRA |= _BV(ADSC);
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  sei();
  UCSR0B |= _BV(RXCIE0);
  // inicjalizacja ADC
  adc_init();
  ADCSRA |= _BV(ADSC);
  timer0_init();
  timer2_init();
  char* a;
  char buff[200];
  float target_temp=35.0;
  adc_expected = (target_temp*0.01+0.5)*1024.0/1.1;

  pid_Init(2496,384,92,&pidData);

  while(1) {
    do{
      a=fgets(buff,200,stdin);
    }while(a!=NULL&&a!=buff);
    sscanf(buff,"%f",&target_temp);
    if(target_temp>59) target_temp=59;
    adc_expected = (target_temp*0.01+0.5)*1024.0/1.1;
  }
}

