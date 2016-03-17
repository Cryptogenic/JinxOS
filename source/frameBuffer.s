.section .data
.align 4

/*	
	Structure: FrameBufferData
	Structure containing offsets and information for drawing graphics 
*/

.globl FrameBufferData
FrameBufferData:
	.int 	1024	// 0x00 - Width (Physical)
	.int 	768		// 0x04 - Height (Physical)
	.int 	1024	// 0x08 - Width (Virtual)
	.int 	768		// 0x0C - Height (Physical)
	.int 	0		// 0x10 - GPU Pitch
	.int 	16		// 0x14 - Bit Depth
	.int 	0		// 0x18 - X Component
	.int 	0 		// 0x1C - Y Component
	.int 	0 		// 0x20 - GPU Pointer
	.int 	0		// 0x24 - GPU Size

.section .text

/*	
	Function: InitFrameBuffer
	Initializes a FrameBufferData structure

	Parameters:
		r0 - Width for graphic
		r1 - Height for graphic
		r2 - BitDepth for graphic (should be 16 for full colour)

	Returns:
		0   - If failed
		[x] - Address of frame buffer
*/

.globl InitFrameBuffer
InitFrameBuffer:
	width 		.req	r0			// Set r0 -> width
	height 		.req	r1 			// Set r1 -> height
	bitDepth	.req	r2 			// Set r2 -> bitDepth

	cmp 		width,	#4096 		// Check integrity of width, height, and bitdepth
	cmpls 		height,	#4096
	cmpls		bitDepth, #32

	result		.req	r0 			// If integrity check fails, move 0 into r0, return

	movhi		result,	#0
	movhi		pc,		lr

	push {r4,lr}

	fbDataAddress .req	r4			// Set r4 -> fbDataAddress

	ldr 		fbDataAddress,=FrameBufferData		// Load FrameBufferData structure into fbDataAddress

	str			width,	[fbDataAddress,#0]		// Store fbDataAddress + 0x00 (width physical) into width
	str			height,	[fbDataAddress,#4]		// Store fbDataAddress + 0x04 (height physical) into height
	str			width,	[fbDataAddress,#8]		// Store fbDataAddress + 0x08 (width virtual) into width
	str			height, [fbDataAddress,#12] 	// Store fbDataAddress + 0x0C (height virtual) into height
	str			bitDepth, [fbDataAddress,#20] 	// Store fbDataAddress + 0x14 (bit depth) into bitDepth

	.unreq		width 				// Set width -> r0 as we no longer need it
	.unreq		height 				// Set height -> r1 as we no longer need it
	.unreq		bitDepth 			// Set bitDepth -> r2 as we no longer need it

	mov 		r0,		fbDataAddress 				// Move the address into r0 (return register)
	add 		r0,		#0x40000000 				// Add 0x40000000 to the address
	mov 		r1,		#1 							// Move #1 into r1 as we're writing to mailbox 1
	bl 			MailWrite 							// Write to mailbox for GPU

	mov 		r0,		#1			// Move #1 into r0 for MailRead as we're reading from mailbox 1
	bl 			MailRead 			// Branch to MailRead, fetch message in r0 (return register)

	teq 		result,	#0 			// Test if our result is 0, if so, failed, return 0.
	movne		result,	#0
	popne		{r4,pc}

	mov 		result,	fbDataAddress				// Otherwise, move address and return it for usage
	pop 		{r4,pc}
	.unreq		result
	.unreq		fbDataAddress

