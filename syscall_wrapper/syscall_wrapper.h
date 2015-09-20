#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 *	This wrapper will ease the invocation of newly added system calls
 *	Think of it as a part of glibc
 */
#define CLONE_NEWTIME 0x02000000

int gettimeofday(struct timeval *, struct timezone *);
int set_tdf(int, int);

