#include <sys/stat.h>

caddr_t _sbrk(int incr)
{
	extern char _end;
	static char *heap_end = 0;
	char *prev_heap_end;

	if(heap_end == 0)
	{
		heap_end = &_end;
	}

	prev_heap_end = heap_end;

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}