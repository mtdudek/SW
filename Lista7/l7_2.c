////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 7, zadanie 2		////
////////////////////////////////////////////
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include "i2c.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>1 
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

const char st1[] PROGMEM = "write\0";
const char st2[] PROGMEM = "read\0";

#define i2cCheck(code, msg) \
  if ((TWSR & 0xf8) != (code)) { \
    printf(msg " failed, status: %.2x\r\n", TWSR & 0xf8); \
    i2cReset(); \
    continue; \
}

const uint8_t eeprom_addr = 0xa0;

uint8_t var[255];

uint16_t convert(char a){
  if('0'<=a&&a<='9')
	  return a-'0';
  if('A'<=a&&a<='F')
	  return a-'A'+10;
}

void i2cReadStart(uint16_t addr){
  for(char i=0;i<1;i++){
	i2cStart();
	i2cCheck(0x08, "I2C read start")
	i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
	i2cCheck(0x18, "I2C EEPROM write request")
	i2cSend(addr & 0xff);
	i2cCheck(0x28, "I2C EEPROM set address")
	i2cStart();
	i2cCheck(0x10, "I2C second start")
	i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
	i2cCheck(0x40, "I2C EEPROM read request")
  }
}

int I8HEX(uint16_t addr,uint8_t length){  //zwraca kolejny adres
  uint8_t sum=0,data;
  if(addr!=0x100&&addr<0x1ff)
	i2cReadStart(addr);
  printf(":%.2X%.4X00",length,addr);
  sum=length+(addr>>8)+(addr&0xff);
  for (int i=0;i<length;i++){
	data=0;
	if(addr == 0x100){
		i2cReadStart(addr);
	}
	if(addr==0x1fff||addr==0xff||i==length-1){
	  data=i2cReadNoAck();
	  i2cCheck(0x58, "I2C EEPROM read")
      i2cStop();
      i2cCheck(0xf8, "I2C stop")
	  _delay_ms(5);
    } else if(addr<=0x1ff){
	  data=i2cReadAck();
	  i2cCheck(0x50, "I2C EEPROM read")
	}
	addr++;
	printf("%.2X",data);
	sum+=data;
  }
  sum=~sum;
  sum++;
  printf("%.2X\n\r",sum);
  for(char i=0;i<1;i++){
    i2cStop();
    i2cCheck(0xf8, "I2C stop")
	_delay_ms(5);
  }
  return addr;
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  sei();
  UCSR0B |= _BV(RXCIE0);
  // zainicjalizuj I2C
  i2cInit();
  // program testowy
  uint16_t addr = 0, length = 0;
  uint8_t data,t;
  char* a;
  char inst[10],buff[600];
  while(1) {
    do{
      a=fgets(&buff,600,stdin);
    }while(a!=NULL&&a!=&buff);
    printf("%s\r",buff);
    t=sscanf(&buff,"%s%x%x",&inst,&addr,&length);
    if(0==strcmp_P(inst,&(st1))){
      if(t==3){
        data=length;
        i2cStart();
        i2cCheck(0x08, "I2C write start")
        i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
        i2cCheck(0x18, "I2C EEPROM write request")
        i2cSend(addr & 0xff);
        i2cCheck(0x28, "I2C EEPROM set address")
        i2cSend(data);
        i2cCheck(0x28, "I2C EEPROM data transmitted")
        i2cStop();
        i2cCheck(0xf8, "I2C stop")
      } else if(t==1){
		char c;
		while(1){
		  do{
		  a=fgets(&buff,600,stdin);
		  }while(a!=NULL&&a!=&buff);
		  printf("%s\r",buff);
		  uint8_t mode=0,sum=0,control_sum=0;
		  c=buff[0];
		  if(c!=':'){
			printf("Bad begining\n\r");
		    break;
		  }
		  length=convert(buff[2])+16*convert(buff[1]);				
		  addr=convert(buff[6])+16*convert(buff[5])+(uint16_t)256*convert(buff[4])+(uint16_t)4096*convert(buff[3]);
		  mode=convert(buff[8])+16*convert(buff[7]);
          uint16_t i=10;
		  sum=length+(addr>>8)+(addr&0xff)+mode;
		  for (int j=0;j<length;i+=2){
			var[j]=convert(buff[i])+16*convert(buff[i-1]);
			sum+=var[j];
			j++;
		  }
		  control_sum=convert(buff[i])+16*convert(buff[i-1]);
		  if((uint8_t)(sum+control_sum)){
		    printf("Bad Data\n\r");
		    break;
		  }
	      if(mode==0x01){
		    printf("End of Data\n\r");
		    break;
		  }
		  for(uint16_t j=0;j<(uint16_t)length;j++){
			if(addr+j<=0x1ff){
			  uint16_t h=addr+j;
			  i2cStart();
			  i2cCheck(0x08, "I2C write start")
			  i2cSend(eeprom_addr | ((h & 0x100) >> 7));
			  i2cCheck(0x18, "I2C EEPROM write request")
			  i2cSend(h & 0xff);
			  i2cCheck(0x28, "I2C EEPROM set address")
			  i2cSend(var[j]);
			  i2cCheck(0x28, "I2C EEPROM data transmitted")
			  i2cStop();
			  i2cCheck(0xf8, "I2C stop")
			  _delay_ms(5);
			}
		  }
		}
	  }
    } else if(0==strcmp_P(inst,&(st2))){
      if(t==2){
		 printf("OK\n\r");
		data=0;
		if(addr<=0x1ff){
		  i2cReadStart(addr);
		  data = i2cReadNoAck();
		  i2cCheck(0x58, "I2C EEPROM read")
		  i2cStop();
		  i2cCheck(0xf8, "I2C stop")
		}
		printf("%.3x: %x\r\n", addr, data);
	  }
	  else if (t==3){
		  while(length>0xff){
			addr=I8HEX(addr,0xff);
			length-=0xff;
		  }
		  if(length>0)
			I8HEX(addr,length);
		  printf(":00000001FF\n\r");
	  }
    }
  }
}
