////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 8, zadanie 3		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>

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
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); 
}

volatile float target_temp=35.0,
	       temp=0.0;

ISR(ADC_vect){
  uint16_t v = ADC; 
  temp=(((float)v*1.1/(float)1024))/0.01;
  //if(temp>=target_temp){
    //PORTB &= ~_BV(PB5);
  //}
  //else if(temp<target_temp-3.0){
    //PORTB |= _BV(PB5);
  //}
}

void timer0_init(){
  OCR0A=255;
  TCCR0A |= _BV(WGM01);
  TIMSK0 |= _BV(OCIE0A);
  TCCR0B |= _BV(CS02) | _BV(CS00);
}

ISR(TIMER0_COMPA_vect){
  printf("T %.2fC\r",temp);
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  DDRB |= _BV(PB5);
  sei();
  UCSR0B |= _BV(RXCIE0);
  // inicjalizacja ADC
  adc_init();
  timer0_init();
  char* a;
  char buff[200];
  ADCSRA |= _BV(ADSC);
  while(1) {
    do{
      a=fgets(buff,200,stdin);
    }while(a!=NULL&&a!=buff);
    sscanf(buff,"%f",&target_temp);
  }
}

