#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include "util.h"

long int issue_freeze(pid_t pid)
{
    long int i, usec;
    struct timeval prev, next, diff;
    for ( i = 0; i < NR_NOP_ROUND / 2; ++i) {
        // do nothing
    }
    gettimeofday(&prev, NULL);
    freeze_proc(pid);
    for ( i = 0; i < NR_NOP_ROUND; ++i ) {
        // pause for a while
    }
    unfreeze_proc(pid);
    gettimeofday(&next, NULL);

    timeval_substract(&diff, &next, &prev);
    usec = timeval_to_usec(diff);
    printf("I make %d freeze %ld micro_sec\n", pid, usec);
    return usec;
}

int main(int argc, char** argv)
{
    int pid_found, pid;
    int opt;
    const char* const short_options = "np:";
    const struct option long_options[] = {
        {"nop", 0, 0, 0},
        {"pid", 1, 0, 0}
    };
    pid_found = 0;
    
    do {
        opt = getopt_long(argc, argv, short_options, long_options, NULL);
        switch (opt) {
            case 'n':
                break;
            case 'p':
                pid = atoi(optarg);
                pid_found = 1;  
                break;
            case -1:
                break;
            default:
                printf("Usage: %s -p pid\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    while (opt != -1);

    if ( pid_found == 1 ) {
        issue_freeze(pid);
    }

    return 0;
}

