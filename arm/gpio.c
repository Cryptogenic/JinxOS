#include <sys/gpio.h>

volatile unsigned int *gpio_registers;

void gpio_init()
{
	gpio_registers = (unsigned int *)GPIO_BASE;
}

void gpio_write(int bit, int value)
{
	gpio_registers[bit] = value;
}

void gpio_set_flag(int bit, int value)
{
	gpio_registers[bit] |= value;
}

int gpio_get_flag(int bit)
{
	return gpio_registers[bit];
}