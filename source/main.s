.section .init
.globl _start
_start:

b main

.section .text
main:
	mov			sp,		#0x8000		// Move 0x8000 into the stack pointer (give stack some space)

	mov 		r0,		#1024		// Move value of 1024d into r0 (width of graphic)
	mov			r1,		#768		// Move value of 768d into r1 (height of graphic)
	mov 		r2,		#16			// Move value of 16d into r2 (bitDepth, 16 = full colour)

	bl 			InitFrameBuffer		// Branch and initialize the frame buffer

	teq			r0,		#0			// Test if result is 0, if not, everything is OK
	bne 		postGpuPass$		// Everything OK, branch to postGpuPass$

	mov			r0,		#15			// Tested 0 meaning a problem occured, turn OK ACT ON
	mov			r1,		#1			// Enable pin
	bl			SetGpioACT			// Branch to set the pin
	mov			r0,		#15			// Move 15th bit into r0
	mov			r1,		#0			// Turn pin OFF, effectively turning ACT OK "ON"
	bl 			SetGpioState		// Branch to set the state

	postGpuFail$:					// Graphic failed to draw, loop nothing forever, do not continue
		b 		postGpuFail$

	postGpuPass$:					// Graphic passed, draw graphic
		fbDataAddress		.req	r4		// Set r4 -> fbDataAddress
		mov					fbDataAddress,r0

render$:
	fbAddress	.req	r3			// Set r3 -> dbAddress
	ldr			fbAddress, [fbDataAddress,#32]	// Load dbDataAddress + 0x20 (GPU Pointer) into r3
	//sub 		fbAddress, #0xC0000000 				// Fixes black screen issue

	colour		.req	r0			// Set r0 -> colour
	y			.req	r1			// Set r1 -> y
	mov			y,		#768		// Move 768d into y (r1)

	drawRow$:						// Draw row of pixels
		x		.req	r2			// Set r2 -> x
		mov		x,		#1024		// Move 1024d into x (r2)

		drawPixel$:					// Draw pixel across rows
			strh		colour,	[fbAddress]		// Store fbAddress as a halfword 
			add 		fbAddress, 	#2			// Add 2d to the dbAddress
			sub 		x,		#1				// Subtract 1d from X
			teq			x,		#0				// Test if X is equal to 0 yet
			bne 		drawPixel$				// If not, reloop drawPixel$

		sub		y,		#1			// Subtract 1 from the y value
		add 	colour,	#1			// Add 1 to the colour value
		teq		y,		#0			// Test if Y is equal to 0 yet
		bne		drawRow$			// If not, reloop drawRow$

	b 			render$				// We have finished, keep looping render$ forever

	.unreq 		fbAddress 			// Set fbAddress -> r3
	.unreq 		fbDataAddress		// Set dbDataAddress -> r4

