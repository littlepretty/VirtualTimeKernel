#include <unistd.h>		// for getpid()
#include <sys/types.h>		// for wait()
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vtutil.h"

long int without_virtual_time()
{
        struct timeval prev, next, diff, tmp;
        long int i, usec;

        gettimeofday(&prev, NULL);
        for (i = 0; i < NR_GTOD_ROUND; ++i) {
                gettimeofday(&tmp, NULL);
        }
        gettimeofday(&next, NULL);
        timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Elapsed %ld micro_sec without virtual time\n", usec);
        return usec;
}

long int with_virtual_time(int dilation)
{
        struct timeval prev, next, diff, tmp;
        int ret, status;
        long int i, usec;
        pid_t pid;

        pid = fork();
        ret = gettimeofday(&prev, NULL);
        if (pid == -1) {
                printf("\n[error] fork fails with error: %s\n", strerror(errno));
        } else if (pid == 0) {
                // child process
                pid_t self = getpid();
                virtual_time_unshare(CLONE_NEWNET|CLONE_NEWNS);
                set_new_dilation(self, dilation);
                for (i = 0; i < NR_GTOD_ROUND; ++i) {
                        ret = gettimeofday(&tmp, NULL);
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
                gettimeofday(&next, NULL); 
                ret = timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                printf("Elapsed %ld micro_sec with virtual time\n", usec);
                return usec;
        }
}

int main(int argc, char const *argv[])
{
        int dilations[] = {1,2,4,8,16};
        size_t size = sizeof(dilations) / sizeof(dilations[0]);
        int i;
        long int noVT, VT;
        float avg;

        for (i = 0; i < size; ++i) {
                noVT = without_virtual_time();
                VT = with_virtual_time(dilations[i]);
                avg = ((float)(VT - noVT)) / NR_GTOD_ROUND;
                printf("[Dilation(%d)] Total/Average overhead = %ld(%.3f) microseconds for %ld GTOD\n", dilations[i], VT - noVT, avg, NR_GTOD_ROUND);
        }
        return 0;
}


