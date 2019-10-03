////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 8, zadanie 2		////
////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

uint8_t pos=0;

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
  // program testowy
  uint8_t address=0b00000000,
	  data=0b00100000;
  for (uint8_t j=0;j<6;j++){
    for (uint8_t i=0;i<8;i++){
      LCD_WriteCommand( HD44780_CGRAM_SET | (address+i));
      LCD_WriteData(data);
    }
    address+=1<<3;
    data+=1<<(4-j);}
  uint8_t prog=0;
  while(1) {
    if(prog==81)
      prog=0;
    LCD_Clear();
    LCD_GoTo(0,0);
    printf("Progress %d",prog);
    uint8_t pom=prog;
    LCD_GoTo(0,1);
    while(pom>=5){
      LCD_WriteData(5);
      pom-=5;
    }
    LCD_WriteData(pom);
    _delay_ms(250);
    prog++;
  }
}

