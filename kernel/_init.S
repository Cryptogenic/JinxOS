.section ".text.startup"

.global _start
.global _get_stack_pointer

_start:
	ldr sp, =(64 * 1024 * 1024)
	b _cstartup

_get_stack_pointer:
	str sp, [sp]
	ldr r0, [sp]

	mov pc, lr

_inf_loop:
	b _inf_loop