#include "errors.h"


void fail(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fflush(stdout);
	exit(EXIT_FAILURE);
}

