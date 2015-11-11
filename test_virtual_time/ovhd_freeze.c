#include <unistd.h>		// for getpid()
#include <sys/types.h>		// for wait()
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vtutil.h"

void repeat_freeze()
{
        long int i, usec;
        pid_t self;

        virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);
        self = getpid();
        for (i = 0; i < NR_SET_ROUND; ++i) {
                (self, i % 4);
        }
        printf("Finish %lu SET dilations\n", NR_SET_ROUND);
}

int main(int argc, char const *argv[])
{
        long int usec;
        
        repeat_freeze();
        
        // usec = repeat_set_dilation_for_myself();
        // printf("Total Overhead = %ld micro_sec for %ld SET dilations\n", usec, NR_SET_ROUND);
        // printf("Avg Overhead = %f micro_sec\n", (float)usec / NR_SET_ROUND);
        
        // usec = repeat_set_dilation_for_my_parent();
        // printf("Avg overhead %f micro_sec for my parent\n", (float)usec / NR_SET_ROUND);
        return 0;
}
