////////////////////////////////////////////
//// Maciej Dudek , nr indeksu: 299186 	////
//// 	     Systemy Wbudowane		////
////	     Lista 12, zadanie 1	////
////////////////////////////////////////////

/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY   1

#define mainSERIAL_TASK_PRIORITY 1

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void vButtonLED(void* pvParameters);

static void vLEDBar(void* pvParameters);

/******************************************************************************
 * Public function definitions.
 ******************************************************************************/

#define LEDs_DDR DDRB
#define LEDs_PORT PORTB
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define LEDstrip_DDR DDRD
#define LEDstrip_PORT PORTD

/**************************************************************************//**
 * \fn int main(void)
 *
 * \brief Main function.
 *
 * \return
 ******************************************************************************/
int main(void)
{
    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle serial_handle;

    LEDs_DDR |= 0b00000001;
    LEDs_PORT &= 0b00000000;
    BUTTON_DDR &= 0b11111110;
    BUTTON_PORT |= 0b00000001;
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    LEDstrip_DDR |= 0b11111111;
    LEDstrip_PORT |= 0b10000000;

    xTaskCreate
        (
         vButtonLED,
         "buttonLED",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &blink_handle
        );

    xTaskCreate
        (
         vLEDBar,
         "LEDBar",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainSERIAL_TASK_PRIORITY,
         &serial_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

/**************************************************************************//**
 * \fn static vApplicationIdleHook(void)
 *
 * \brief
 ******************************************************************************/
void vApplicationIdleHook(void)
{

}

/******************************************************************************
 * Private function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn static void vBlinkLed(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vButtonLED(void* pvParameters)
{
    uint8_t tab[1000];
    uint8_t num = 0;
    for ( ;; )
    {
        if(num>=1000)
          num-=1000;
        if(tab[num])
          LEDs_PORT |=0b00000001;
        else
          LEDs_PORT &=0b11111110;
        //czy naciśnięto guzik 
        if (!(PINC & 0b00000001) ){
          tab[num]=1;
        } else 
          tab[num]=0;
        num++;
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}


/**************************************************************************//**
 * \fn static void vSerial(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vLEDBar(void* pvParameters)
{
    for ( ;; )
    {
      for(int i=0;i<7;i++){
        LEDstrip_PORT >>=1;
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
      for(int i=0;i<7;i++){
        LEDstrip_PORT <<=1;
        vTaskDelay(100 / portTICK_PERIOD_MS);
      } 
    }
}
