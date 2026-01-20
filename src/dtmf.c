#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

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