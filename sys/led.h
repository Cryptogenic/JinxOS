#ifndef _SYS_LED_H
#define _SYS_LED_H

#include <sys/gpio.h>

#if defined(RPIBPLUS) || defined(RPI2)
	#define LED_GPFSEL      GPIO_GPFSEL4
    #define LED_GPFBIT      21
    #define LED_GPSET       GPIO_GPSET1
    #define LED_GPCLR       GPIO_GPCLR1
    #define LED_GPIO_BIT    15
#else
    #define LED_GPFSEL      GPIO_GPFSEL1
    #define LED_GPFBIT      18
    #define LED_GPSET       GPIO_GPSET0
    #define LED_GPCLR       GPIO_GPCLR0
    #define LED_GPIO_BIT    16
#endif

void led_toggle_on();
void led_toggle_off();

#endif