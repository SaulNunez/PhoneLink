#include <stdio.h>
#include "driver\gpio.h"
#include "freeRTOS\freeRTOS.h"
#include "freeRTOS\task.h"

#define PIN_1_DTMF 1
#define PIN_2_DTMF 2
#define PIN_3_DTMF 3
#define PIN_4_DTMF 4

void onPinUpdate(){
    gpio_set_direction(PIN_1_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_2_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_3_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_4_DTMF, GPIO_MODE_INPUT);
}