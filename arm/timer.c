#include <sys/timer.h>

static arm_timer_t *arm_timer = (arm_timer_t *)TIMER_ARM_BASE;
static sys_timer_t *system_timer = (sys_timer_t *)TIMER_SYS_BASE;

void sys_timer_wait(uint32_t us)
{
	volatile uint32_t time = system_timer->counter_lo;

	while((system_timer->counter_lo - time) < us)
	{
		// Busy Wait
	}
}