/*	
	Function: GetSystemTimerAddress
	Returns the base system timer from hardware

	Returns:
		r0 - Base address for system timer address
*/

.globl GetSystemTimerAddress
GetSystemTimerAddress:
	ldr 	r0,		=0x20003000		// Base Address of the System Timer
	mov 	pc,		lr 				// Return, move link register into pointer control

/*
	Function: GetTimeStamp
	Returns the timestamp of the system

	Returns:
		r0 - Timestamp of the RPi
*/

.globl GetTimeStamp
GetTimeStamp:
	push 	{lr}					// Push link register unto the stack for recovery
	bl 		GetSystemTimerAddress		// Branch to GetSystemTimerAddress to get the address
	ldrd 	r0,		r1,		[r0,#4]		// Load the address from r0 + 0x04 across r0 and r1
	pop 	{pc}					// Pop pointer controller off the stack into pc register

/*
	Function: Delay
	Delays for the milliseconds specified in r0

	Parameters:
		r0 - Milliseconds to delay
*/

.globl Delay
Delay:
	delay 	.req	r2				// Set r2 -> delay
	mov 	delay,	r0				// Move r0 (milliseconds) into delay (r2)
	push 	{lr}					// Push link register value unto stack for recovery
	bl 		GetTimeStamp			// Branch to GetTimeStamp to fetch the current time stamp
	start 	.req	r3				// Set r3 -> start
	mov 	start,	r0				// Move r0 (timestamp) into start (r3)

	timeLoop$:						// Run a loop to check time elapsed
		bl 			GetTimeStamp	// Branch to GetTimeStamp to fetch the current time stamp
		elapsed 	.req		r1			// Set r1 -> elapsed
		sub 		elapsed, 	r0,	start	// Subtract r0 from start, store in elapsed (r1)
		cmp 		elapsed, 	delay 		// Compare elapsed and delay
		.unreq 		elapsed 		// Set elapsed -> r1, we don't need it anymore
		ble 		timeLoop$		// If elapsed < delay, loop

	.unreq 		delay 				// Set delay -> r0
	.unreq		start 				// Set start -> r3
	pop 		{pc}				// Pop pointer control back to pc register off the stack

