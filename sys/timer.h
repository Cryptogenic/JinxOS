#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

#include <stdint.h>

#ifdef RPI2
    #define TIMER_ARM_BASE   0x3F00B400UL
	#define TIMER_SYS_BASE 	 0x3F003000UL
#else
    #define TIMER_ARM_BASE   0x2000B400UL
	#define TIMER_SYS_BASE   0x20003000UL
#endif

#define TIMER_CTRL_23BIT 		(1 << 1)

#define TIMER_CTRL_PRESCALE_1 	(0 << 2)
#define TIMER_CTRL_PRESCALE_16 	(1 << 2)
#define TIMER_CTRL_PRESCALE_256 (2 << 2)

#define TIMER_CTRL_ENABLE 		(1 << 7)
#define TIMER_CTRL_DISABLE 		(0 << 7)

#define TIMER_CTRL_INT_ENABLE 	(1 << 5)
#define TIMER_CTRL_INT_DISABLE 	(0 << 5)

// BCM2835 Documentation details layout
typedef struct {
	volatile uint32_t Load;
	volatile uint32_t Value;
	volatile uint32_t Control;
	volatile uint32_t IRQClear;
	volatile uint32_t RAWIRQ;
	volatile uint32_t MaskedIRQ;
	volatile uint32_t Reload;
	volatile uint32_t PreDivider;
	volatile uint32_t FreeRunningCounter;
} arm_timer_t;

typedef struct {
	volatile uint32_t control_status;
	volatile uint32_t counter_lo;
	volatile uint32_t counter_hi;
	volatile uint32_t compare0;
	volatile uint32_t compare1;
	volatile uint32_t compare2;
	volatile uint32_t compare3;
} sys_timer_t;

extern void sys_timer_wait(uint32_t us);

#endif