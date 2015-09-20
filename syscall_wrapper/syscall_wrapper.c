#include <sys/syscall.h>

// int timeclone(unsigned long flags, int dilation)
// {
//     return syscall(317, flags | 0x02000000, 0, NULL, dilation, NULL);
// }

/*
 * hard coded number from syscall_64.tbl
 */
#ifndef __NR_gettimeofday
	#define __NR_gettimeofday 96
#endif

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    return syscall(__NR_gettimeofday, tv, tz); // 96 is from syscall_64.tbl
}


#ifndef __NR_set_tdf
	#define __NR_set_tdf 317
#endif
/*
 *  ppid == 0 : change caller itself's dilation
 *  ppid !=0 :  change caller's parent's dilation, also double check ppid
 */
int set_tdf(int dilation, int ppid)
{
	return syscall(__NR_set_tdf, dilation, ppid);
}

