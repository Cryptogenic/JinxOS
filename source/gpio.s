/*	
	Function: GetGpioAddress
	Returns the base gpio controller address from hardware

	Returns:
		r0 - Base address for gpio controller
*/

.globl GetGpioAddress
GetGpioAddress:
	ldr 	r0,	 =0x20200000		// Base Address of gpio controller
	mov 	pc,	 lr 				// Return, move link register into pointer control

/*
	Function: SetGpioACT
	Sets the GPIO OK ACT Pin

	Parameters:
		r0 - Pin Number
		r1 - Pin Value
*/

.globl SetGpioACT
SetGpioACT:
	cmp 	r0, 	#53				// Compares r0 (pin number) with 53d
	cmpls 	r1,	 	#7				// Compares 7 with r1 if r0 < 53
	movhi 	pc,		lr 				// Any condition fail above will move link register to pc, return

	push 	{lr} 					// Push link register unto the stack for recovery
	mov 	r2,		r0				// Move pin number (r0) into register 2
	bl 		GetGpioAddress			// Branch to GetGpioAddress to get GPIO Base Address, stored in register 0

	integrityLoop$:					// Loop for an integrity check of arguments
		cmp 	r2,		#9			// Compares register 2 (value) with 9d
		subhi 	r2,		#10			// Subtract 10d from r2 if high
		addhi 	r0,		#4			// Add 4 to r0 if high
		bhi 	integrityLoop$		// If still high, run integrity loop again

	add 	r2, 	r1,	lsl #1		// Logical shift left 1d, add it to register 1, and store in register 2
	lsl 	r1,		r2 				// Logical shift r2 << and store in r1
	str 	r1,		[r0] 			// Store r0 in r1
	pop 	{pc} 					// Pop pointer control off the stack into pc register

/*
	Function: SetGpioState
	Sets the specified GPIO pin to the specified state

	Parameters:
		r0 - Pin Number
		r1 - Pin Value (State)
*/

.globl SetGpioState
SetGpioState:
	pinNum		.req	r0 			// Set r0 -> pinNum
	pinVal 		.req 	r1 			// Set r1 -> pinVal

	cmp 		pinNum,	#53 		// Compare pinNum (r0) with 53d
	movhi 		pc,		lr 			// If high flag set, exit, invalid parameter
	push 		{lr}				// Push link register unto the stack for recovery
	mov 		r2,		pinNum		// Move pinNum (r0) into r2
	.unreq 		pinNum				// Set pinNum -> r0 as we need to reset it as register 2
	pinNum	 	.req 	r2			// Set r2 -> pinNum
	bl 			GetGpioAddress		// Branch to get GPIO Controller Address, will be stored in r0
	gpioAddr 	.req 	r0			// Set r0 -> GPIO Controller Address fetched from GetGpioAddress

	pinBank 	.req 	r3			// Set r3 -> pinBank
	lsr 		pinBank,pinNum,#5
	lsl 		pinBank, #2
	add 		gpioAddr,pinBank
	.unreq 		pinBank

	and 		pinNum,	#31
	setBit 		.req 	r3
	mov 		setBit,	#1
	lsl 		setBit, pinNum
	.unreq 		pinNum

	teq 		pinVal,	#0			// Test using bitwise & with pin value and 0x00
	.unreq	 	pinVal				// Set pinVal -> r1 as we don't need it anymore
	streq 		setBit, [gpioAddr,#32]	// Set pin to an ON state, turning the LED OFF
	strne 		setBit, [gpioAddr,#44]	// Set pin to an OFF state, turning the LED ON
	.unreq 		setBit				// Set setBit -> r3 as we don't need it anymore
	.unreq 		gpioAddr 			// Set gpioAddr -> r0 as we don't need it anymore
	pop 		{pc}				// Pop pointer control back to pc register off the stack

