/*	
	Function: GetMailAddress
	Returns the base mail address from hardware

	Returns:
		r0 - Base address for mail for GPU <-> CPU interaction
*/

.globl GetMailAddress
GetMailAddress:
	ldr		r0,	=0x2000B880 		// Base Address of the mail hardware component
	mov 	pc,	lr 					// Return, move link register into pointer control

/*	
	Function: MailWrite
	Writes to mail for GPU <-> CPU interaction

	Parameters:
		r0 - Message to write
		r1 - Mailbox # to write to
*/

.globl MailWrite
MailWrite:
	tst 		r0,		#0b1111 	// Bitmask to check if r0 is zero'd out
	movne 		pc,		lr 	 		// If r0 != 0, invalid arguments, return
	cmp 		r1,		#15	 		// Compare mailbox number with 15[d]
	movhi 		pc,		lr   		// If mailbox number != 15, invalid arguments, return

	channel 	.req 	r1	 		// Set r1 -> channel
	value 		.req 	r2   		// Set r2 -> value

	mov 		value,	r0	 		// Move parameter of what to write into value (r2)
	push 		{lr}		 		// Store the link register on the stack for recovery

	bl			GetMailAddress 		// Branch to GetMailAddress to get base mail address, returned in r0
	mailbox 	.req 	r0	   		// Set r0 -> mailbox as it contains our mail address from GetMailAddress

	waitStatusW$:			 		// Loop to wait on a status update
		status 	.req 	r3   				// Set r3 -> status
		ldr		status,	[mailbox,#0x18]		// Set status to the value of mailbox + 0x18 (offset for status)

		tst		status,	#0x80000000		// Test using bitwise & that the top bit of status is 0
		.unreq	status 					// Set status -> r3
		bne		waitStatusW$			// If top bit of status != 0, invalid, reloop

	add 	value,	channel				// Add value and channel for result
	.unreq 	channel						// Set channel -> r1

	str 	value,	[mailbox,#0x20]		// Store mailbox + 0x20 (offset for top bit of status (2 bytes))
	.unreq 	value						// Set value -> r2, mailbox -> r0
	.unreq 	mailbox
	pop 	{pc}					// Pop pointer control back to pc register off the stack

/*	
	Function: MailRead
	Reads from mail for GPU <-> CPU interaction

	Parameters:
		r0 - Mailbox # to read from
	
	Returns:
		r0 - Mail message read
*/

.globl MailRead
MailRead:
	cmp 	r1,	#15					// Integrity check, if r1 != 15, move link register into pointer controller, exit
	movhi 	pc,	lr

	channel 	.req 		r1		// Set r1 -> channel
	mov 		channel,	r0		// Move argument of r0 (mailbox #) into channel
	push 		{lr}				// Push link register value unto stack for recovery

	bl 			GetMailAddress		// Branch to GetMailAddress to get base mail address, returned in r0
	mailbox		.req 	r0			// Set mailbox -> r0, as r0 is returned from GetMailAddress

	mailIntegrity$:					// Runs loop to check mail integrity
		waitStatusR$:				// Runs loop to wait for a valid status
			status 	.req 		r2					// Set r2 -> status
			ldr 	status,		[mailbox,#0x18]		// Load status with mailbox + 0x18 (offset for status)

			tst 	status,		#0x40000000			// Test status if the 30th bit is zero
			.unreq 	status 							// Set status -> r2, we don't need it anymore
			bne 	waitStatusR$					// If 30th bit != 0, reloop waitStatusR$

		mail 	.req 	r2					// Set r2 -> mail
		ldr 	mail,	[mailbox,#0]		// Load mailbox + 0x00 (no offset) into mail (r2)

		inchannel 	.req 	r3				// Set r3 -> inchannel
		and 	inchannel,	mail,	#0b1111 // Bit Mask Check to check if channel & mail are zero
		teq 	inchannel,	channel			// Tests equality
		.unreq 	inchannel					// Set inchannel -> r3, we don't need it anymore
		bne 	mailIntegrity$				// If equality != true, reloop mailIntegrity$

	.unreq 		mailbox						// Set mailbox -> r0
	.unreq 		channel						// Set channel -> r1

	and 		r0,		mail,	#0xFFFFFFF0	// Moves top 28 bits into r0 (return register)
	.unreq 		mail 						// Set mail -> r2
	pop 		{pc} 						// Pop pointer control back to pc register off the stack

