#include <stdio.h>
#include "driver\gpio.h"
#include "freeRTOS\freeRTOS.h"
#include "freeRTOS\task.h"

#define HOLD_DETECT GPIO_NUM_10


void phone_setup()
{
    //gpio_set_direction(HOLD_DETECT, GPIO_MODE_INPUT);
    //gpio_set_intr_type(HOLD_DETECT, GPIO_INTR_POSEDGE);
    //gpio_install_isr_service(0);
    //gpio_isr_handler_add(HOLD_DETECT, gpio_isr_handler, NULL);
    //gpio_intr_enable(HOLD_DETECT);
}

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    /* 
        TODO - Implement the debouncing algorithm 
        to eliminate the Spurious interrupt trigger.
    */
   
    /* Disable the Interrupt */
    gpio_intr_disable(HOLD_DETECT);
    gpio_isr_handler_remove(HOLD_DETECT);

    /* Do stuff */

    /* Re-Enable the Interrupt */
    gpio_isr_handler_add(HOLD_DETECT, gpio_isr_handler, NULL);
    gpio_intr_enable(HOLD_DETECT);
}
