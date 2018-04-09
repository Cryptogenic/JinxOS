#include <sys/gpio.h>
#include <sys/led.h>
#include <sys/timer.h>

volatile unsigned int tim;

/*
 * Called by IPL on the SoC, kernel entry point.
 * This function *should* never return!
 */
void kernel_init(unsigned int r0, unsigned int r1, unsigned int atags)
{
	/*
	 * Writes 1 to the GPIO16 nibble in the GPIO
	 * peripheral register to enable GPIO16 for
	 * output.
	 */

	gpio_init();
	gpio_set_flag(LED_GPFSEL, (1 << LED_GPFBIT));

	// Indicate we're turned on
	

	while(1)
	{
		sys_timer_wait(500000);

		led_toggle_on();

		sys_timer_wait(500000);

		led_toggle_off();
	}
}