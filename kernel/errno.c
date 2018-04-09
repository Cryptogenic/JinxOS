#include <sys/errno.h>

int errno = 0;

int error(int err)
{
	errno = err;
	return errno;
}