#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()

#include "util.h"

void run_long_loop()
{
    struct timeval prev, next, diff, tmp;
    int ret;
    long int i, usec;

    pid_t pid = getpid();
    printf("****** Process[%d] to be freezed ******\n", pid);
    // child process
    gettimeofday(&prev, NULL);
    for ( i = 0; i < 4 * NR_NOP_ROUND; ++i ) {
        // do nothing
    }
    gettimeofday(&next, NULL);
    ret = timeval_substract(&diff, &next, &prev);
    usec = timeval_to_usec(diff);
    printf("I elapsed %ld micro_sec freeze\n\n", usec);
}

int main()
{
    run_long_loop();
    return 0;
}
