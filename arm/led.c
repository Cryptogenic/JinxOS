#include <sys/led.h>

void led_toggle_on()
{
	/*
	 * On original PI, setting the LED GPIO to
	 * LOW will cause the LED to turn ON, on
	 * plus models it will cause the LED to turn
	 * OFF
	 */

	#if defined(RPIBPLUS) || defined(RPI2)
		gpio_write(LED_GPSET, (1 << LED_GPIO_BIT));
	#else
		gpio_write(LED_GPCLR, (1 << LED_GPIO_BIT));
	#endif
}

void led_toggle_off()
{
	/*
	 * On original PI, setting the LED GPIO to
	 * HIGH will cause the LED to turn OFF, on
	 * plus models it will cause the LED to turn
	 * ON
	 */

	#if defined(RPIBPLUS) || defined(RPI2)
		gpio_write(LED_GPCLR, (1 << LED_GPIO_BIT));
	#else
		gpio_write(LED_GPSET, (1 << LED_GPIO_BIT));
	#endif
}