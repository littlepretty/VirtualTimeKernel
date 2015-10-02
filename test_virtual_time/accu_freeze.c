#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>           // for timeval
#include <unistd.h>         // for getpid()
#include <sys/types.h>      // for wait()
#include <sys/wait.h>
#include <errno.h>          // for strerror()

#include "util.h"

long int nop_without_freeze()
{
    struct timeval prev, next, diff, tmp;
    int ret, status;
    long int i, usec;
    pid_t pid;

    pid = fork();
    if ( pid == -1 ) {
        printf("\n[error] fork fails with error: %s\n", strerror(errno));
    } else if ( pid == 0 ) {
        // child process
        gettimeofday(&prev, NULL);
        for ( i = 0; i < 4 * NR_NOP_ROUND; ++i ) {
            // do nothing
        }
        gettimeofday(&next, NULL);
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Child elapsed %ld seconds, %ld useconds\n", diff.tv_sec, diff.tv_usec);
        printf("Child elapsed %ld useconds without freeze\n", usec);
    } else {
        pid = wait(&status);
    }
    return 0;
}

long int nop_with_freeze()
{
    struct timeval prev, next, diff, tmp;
    int ret, status;
    long int i, usec;
    pid_t pid;

    pid = fork();
    if ( pid == -1 ) {
        printf("[error] fork fails with error: %s\n", strerror(errno));
    } else if ( pid = 0 ) {
        // child process do the same thing
        gettimeofday(&prev, NULL);
        for ( i = 0; i < 4 * NR_NOP_ROUND; ++i ) {
             // do nothing
        }
        gettimeofday(&next, NULL);
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Child elapse %ld seconds, %ld useconds\n", diff.tv_sec, diff.tv_usec);
        printf("Child elapse %ld useconds with freeze\n", usec);
    } else {
        // parent process freeze and unfreeze child
        for ( i = 0; i < NR_NOP_ROUND / 2; ++i ) {
            // do nothing
        }
        gettimeofday(&prev, NULL);
        freeze_proc(pid);
        for ( i = 0; i < NR_NOP_ROUND * 2; ++i ) {
            // do nothing
        }
        unfreeze_proc(pid);
        gettimeofday(&next, NULL);
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Parent make child freeze %ld seconds, %ld useconds\n", diff.tv_sec, diff.tv_usec);
        printf("Parent make child freeze %ld useconds\n", usec);
    }
    return usec;
}

int main()
{
    long int usec;

    usec = nop_without_freeze();
    usec = nop_with_freeze();
    return 0;
}
