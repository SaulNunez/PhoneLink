#ifndef __GPIO_H__
#define __GPIO_H__

enum offhook_event_type_t {
    GPIO_PHONE_UNHOOKED_RISING_EDGE = 0,
    GPIO_PHONE_UNHOOKED_FALLING_EDGE = 1,
};

typedef enum offhook_event_type_t offhook_event_type_t;



void gpio_init();

#endif /* __GPIO_H__ */
