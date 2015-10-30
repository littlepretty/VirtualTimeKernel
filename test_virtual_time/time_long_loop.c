#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()

#include "vtutil.h"

struct timeval prev, next, diff, temp;

void run_long_loop(int vt_opt, int duration)
{
        int ret;
        long int i, usec;

        if (vt_opt) virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);

        gettimeofday(&prev, NULL);
        usleep(duration);
        /*for ( i = 0; i < duration * CNT_SLEEP; ++i) {*/
                /*gettimeofday(&temp, NULL);*/
        /*}*/
        gettimeofday(&next, NULL);
        timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("%ld\t", usec);
}

/*
 * To run this program, use 'setsid time_long_loop -v -d duration'
 * so that this program detached from its parent(your shell/tty) and
 * freeze this process will not stop its parent or brothers
 */
int main(int argc, char* argv[])
{ 
        const char* short_options = "vd:";
        int opt, virtual_time_flag, duration;

        virtual_time_flag = 0;
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'v':
                                virtual_time_flag = 1;
                                break;
                        case 'd':
                                duration = atoi(optarg);
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -d microseconds [-v]\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        run_long_loop(virtual_time_flag, duration);

        return 0;
}
