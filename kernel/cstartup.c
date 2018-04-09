extern int __bss_start__;
extern int __bss_end__;

extern void kernel_init(unsigned int r0, unsigned int r1, unsigned int atags);

void _cstartup(unsigned int r0, unsigned int r1, unsigned int r2)
{
	int *bss_start = &__bss_start__;
	int *bss_end   = &__bss_end__;

	while(bss_start < bss_end)
		*bss_start++ = 0;

	kernel_init(r0, r1, r2);

	/* Trap */
	while(1) {}
}