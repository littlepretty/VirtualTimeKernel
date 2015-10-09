#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>           // for timeval
#include <unistd.h>         // for getpid()
#include <sys/types.h>      // for wait()
#include <sys/wait.h>
#include <errno.h>          // for strerror()

#include "vtutil.h"

long int nop_without_freeze()
{
    struct timeval prev, next, diff, tmp;
    int ret, status;
    long int i, usec;
    pid_t pid;
    
    printf("****** Experiment withOUT freeze ******\n");
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
        printf("Child elapsed %ld micro_sec without freeze\n\n", usec);
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

    printf("****** Experiment with freeze ******\n");
    pid = fork();
    if ( pid == -1 ) {
        printf("[error] fork fails with error: %s\n", strerror(errno));
    } else if ( pid == 0 ) {
        // child process do the same thing
        ret = virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);
        
        pid_t self = getpid();
        pid_t pgrpid = getpgid(self);
        printf("Child[%d]'s PGID = %d\n", self, pgrpid);

        gettimeofday(&prev, NULL);
        for ( i = 0; i < 4 * NR_NOP_ROUND; ++i ) {
            // do nothing
        }
        gettimeofday(&next, NULL);
        
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff); 
        printf("Child elapsed %ld micro_sec with freeze\n", usec);
    } else {
        // parent process freeze and unfreeze child
        for ( i = 0; i < NR_NOP_ROUND / 2; ++i ) {
            // do nothing
        }
        
        pid_t self = getpid();
        pid_t pgrpid = getpgid(self);
        printf("Parent[%d]'s PGID = %d\n", self, pgrpid);
        
        gettimeofday(&prev, NULL);
        freeze_proc(pid);
        for ( i = 0; i < NR_NOP_ROUND * 2; ++i ) {
            // do nothing
        }
        unfreeze_proc(pid);
        gettimeofday(&next, NULL);
        
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("Parent make child freeze %ld micro_sec\n", usec);
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
