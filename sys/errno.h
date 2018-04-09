#ifndef _SYS_ERRNO_H
#define _SYS_ERRNO_H

extern int errno;

#define ERR_OK 			0 	/* OK */
#define ERR_PERM 		1 	/* Operatoion not permitted */
#define ERR_NOENT 		2 	/* No such entry */
#define ERR_NOPROC 		3 	/* No process exists for this identifier */
#define ERR_SINT 		4 	/* Interrupted system call */
#define ERR_IO			5 	/* Input/Output (I/O) error */
#define ERR_NOTCONF 	6 	/* Device not configured */
#define ERR_NOEXEC 		7  	/* Entry not executable */
#define ERR_BADFD 		8 	/* Bad file descriptor */
#define ERR_DEADLOCK 	9 	/* Avoiding a resource deadlock */
#define ERR_NOMEM 		10 	/* No memory left to allocate */
#define ERR_ACCESS 		11  /* Operator does not have sufficient permissions */
#define ERR_FAULT 		12 	/* Address fault */
#define ERR_BUSY 		13 	/* Device used by another process */
#define ERR_INVALARG	14 	/* Invalid arguments provided */

int error(int err);

#endif