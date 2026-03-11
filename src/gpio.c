#include "gpio.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "hfp.h"

#define PIN_Q1_DTMF 1
#define PIN_Q2_DTMF 2
#define PIN_Q3_DTMF 3
#define PIN_Q4_DTMF 4
#define PIN_StD_DTMF 5 // Please change this to the correct GPIO pin

static xQueueHandle gpio_evt_queue = NULL;

void read_and_send_dtmf() {
    int q1 = gpio_get_level(PIN_Q1_DTMF);
    int q2 = gpio_get_level(PIN_Q2_DTMF);
    int q3 = gpio_get_level(PIN_Q3_DTMF);
    int q4 = gpio_get_level(PIN_Q4_DTMF);

    int value = q1 + (q2 << 1) + (q3 << 2) + (q4 << 3);

    char dtmf_char = ' ';
    switch (value) {
        case 0: dtmf_char = 'D'; break;
        case 1: dtmf_char = '1'; break;
        case 2: dtmf_char = '2'; break;
        case 3: dtmf_char = '3'; break;
        case 4: dtmf_char = '4'; break;
        case 5: dtmf_char = '5'; break;
        case 6: dtmf_char = '6'; break;
        case 7: dtmf_char = '7'; break;
        case 8: dtmf_char = '8'; break;
        case 9: dtmf_char = '9'; break;
        case 10: dtmf_char = '0'; break;
        case 11: dtmf_char = '*'; break;
        case 12: dtmf_char = '#'; break;
        case 13: dtmf_char = 'A'; break;
        case 14: dtmf_char = 'B'; break;
        case 15: dtmf_char = 'C'; break;
    }

    if (dtmf_char != ' ') {
        hfp_send_dtmf(dtmf_char);
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void dtmf_reader_task(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == PIN_StD_DTMF) {
                // add a small delay to debounce and wait for the signals to be stable
                vTaskDelay(10 / portTICK_PERIOD_MS);
                read_and_send_dtmf();
            }
        }
    }
}

void gpio_init()
{
    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = (1ULL<<PIN_StD_DTMF);
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-down mode
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

    // set direction for Q pins
    gpio_set_direction(PIN_Q1_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_Q2_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_Q3_DTMF, GPIO_MODE_INPUT);
    gpio_set_direction(PIN_Q4_DTMF, GPIO_MODE_INPUT);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(dtmf_reader_task, "dtmf_reader_task", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PIN_StD_DTMF, gpio_isr_handler, (void*) PIN_StD_DTMF);
}