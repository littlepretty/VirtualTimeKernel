#include <unistd.h>		// for getpid()
#include <sys/types.h>		// for wait()
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

void repeat_set_dilation()
{
        long int i, usec;
        pid_t self;

        virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);
        self = getpid();
        for (i = 0; i < NR_SET_ROUND; ++i) {
                set_new_dilation(self, i % 4);
        }
        printf("Finish %lu SET dilations\n", NR_SET_ROUND);
}

long int repeat_set_dilation_for_myself()
{
        struct timeval prev, next, diff, tmp;
        long ret;
        int status;
        long int i, usec;

        pid_t pid = fork();
        if (pid == -1) {
                printf("\n[error] fork fails: %s\n", strerror(errno));
        } else if (pid == 0) {
                ret = virtual_time_unshare(CLONE_NEWNET|CLONE_NEWNS);
                check_syscall_status(ret, "virtual_time_unshare");
                pid_t self = getpid();

                for (i = 0; i < NR_SET_ROUND; ++i) {
                        ret = set_new_dilation(self, i % 4); // change itself
                }
                exit(EXIT_SUCCESS);
        } else {
                ret = gettimeofday(&prev, NULL);
                // check_syscall_status(ret, "gettimeofday");
                pid = wait(&status);
                ret = gettimeofday(&next, NULL);
                // check_syscall_status(ret, "gettimeofday");
                ret = timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                printf("Elapsed %ld micro_sec for change TDF\n", usec);
                return usec;
        }
}

long int repeat_set_dilation_for_my_parent()
{
        struct timeval prev, next, diff, tmp;
        long ret;
        int status;
        long int i, usec;

        pid_t pid = fork();
        if (pid == -1) {
                printf("\n[error] fork fails with error: %s\n", strerror(errno));
        } else if (pid == 0) {
                pid = fork();
                if (pid == -1) {
                        printf("\n[error] fork fails with error: %s\n", strerror(errno));
                } else if (pid == 0) {
                        ret = virtual_time_unshare(CLONE_NEWNET|CLONE_NEWNS);
                        check_syscall_status(ret, "unshare");
                        pid_t ppid = getppid();
                        for (i = 0; i < NR_SET_ROUND; ++i) {
                                ret = set_new_dilation(ppid, 4); // change my parent
                        }
                        exit(EXIT_SUCCESS);
                } else {
                        pid = wait(&status);
                        exit(EXIT_SUCCESS);
                }
        } else {
                ret = gettimeofday(&prev, NULL);
                // check_syscall_status(ret, "gettimeofday");
                pid = wait(&status);
                ret = gettimeofday(&next, NULL);
                // check_syscall_status(ret, "gettimeofday");
                ret = timeval_substract(&diff, &next, &prev); 
                usec = timeval_to_usec(diff);
                printf("Elapsed %ld micro_sec for change TDF\n", usec);
                return usec;
        }
}

int main(int argc, char const *argv[])
{
        long int usec;
        
        repeat_set_dilation();
        
        // usec = repeat_set_dilation_for_myself();
        // printf("Total Overhead = %ld micro_sec for %ld SET dilations\n", usec, NR_SET_ROUND);
        // printf("Avg Overhead = %f micro_sec\n", (float)usec / NR_SET_ROUND);
        
        // usec = repeat_set_dilation_for_my_parent();
        // printf("Avg overhead %f micro_sec for my parent\n", (float)usec / NR_SET_ROUND);
        return 0;
}
