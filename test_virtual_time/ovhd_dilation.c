#include <unistd.h>		// for getpid()
#include <sys/types.h>		// for wait()
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

long int without_virtual_time()
{
        struct timeval prev, next, diff, tmp;
        long ret;
        long int i, usec;

        ret = gettimeofday(&prev, NULL);
        // check_syscall_status(ret, "gettimeofday");

        for (i = 0; i < NR_GTOD_ROUND; ++i) {
                ret = gettimeofday(&tmp, NULL);
                // check_syscall_status(ret, "gettimeofday");
        }

        ret = gettimeofday(&next, NULL);
        // check_syscall_status(ret, "gettimeofday");
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Elapsed %ld micro_sec without virtual time\n", usec);
        return usec;
}

long int with_virtual_time()
{
        struct timeval prev, next, diff, tmp;
        int ret, status;
        long int i, usec;
        pid_t pid;

        pid = fork();
        ret = gettimeofday(&prev, NULL);
        // check_syscall_status(ret, "gettimeofday");
        if (pid == -1) {
                printf("\n[error] fork fails with error: %s\n", strerror(errno));
        } else if (pid == 0) {
                // child process
                pid_t self = getpid();
                ret = virtual_time_unshare(CLONE_NEWNET|CLONE_NEWNS);
                check_syscall_status(ret, "virtual_timeun_share");
                ret = set_new_dilation(self ,4);
                for (i = 0; i < NR_GTOD_ROUND; ++i) {
                        ret = gettimeofday(&tmp, NULL);
                        // check_syscall_status(ret, "gettimeofday");
                }
                exit(EXIT_SUCCESS);
        } else {
                pid = wait(&status);
                // if (status == -1)
                // {
                //     perror("wait error");
                //     return -1;
                // }
                // if (WIFEXITED(status) != 0) {
                //     printf("Child process ended normally; status = %d\n", WEXITSTATUS(status));
                // }
                ret = gettimeofday(&next, NULL);
                // check_syscall_status(ret, "gettimeofday");
                ret = timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                printf("Elapsed %ld micro_sec with virtual time\n", usec);
                return usec;
        }
}

int main(int argc, char const *argv[])
{
        long int noVT = without_virtual_time();
        long int VT = with_virtual_time();
        printf("Total Overhead = %ld micro_sec for %ld GTOD\n", VT - noVT, NR_GTOD_ROUND);
        printf("Avgerage Overhead = %f micro_sec\n", ((float)(VT - noVT)) / NR_GTOD_ROUND);
        return 0;
}


