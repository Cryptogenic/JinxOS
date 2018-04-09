.global _get_system_timer_address
_get_system_timer_address:
	ldr r0, =0x20003000
	mov pc, lr 

.global _get_time_stamp
_get_time_stamp:
	push {lr}
	bl _get_system_timer_address
	ldrd r0, r1, [r0,#4]
	pop {pc}

.global _delay
_delay:
	mov r2, r0
	push {lr}
	bl _get_time_stamp
	mov r3, r0

	timeLoop$:
		bl _get_time_stamp
		sub r1, r0, r3
		cmp r1, r0
		ble timeLoop$

	pop {pc}
