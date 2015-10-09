#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "vtutil.h"

void issue_freeze(pid_t pid, int wait, int pause)
{
        long int i, usec;
        struct timeval prev, next, diff;

        for ( i = 0; i < wait * CNT_SLEEP; ++i) {
                // do nothing
        }
        gettimeofday(&prev, NULL);
        freeze_proc(pid);
        for ( i = 0; i < pause * CNT_SLEEP; ++i ) {
                // pause for a while
        }
        unfreeze_proc(pid);
        gettimeofday(&next, NULL);
        timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("%lu\t", usec);

}

#define MAX_NUM_PIDS 256

int main(int argc, char** argv)
{
        int pid_found, opt, index, i, wait, freeze;
        int pid;
        const char* const short_options = "np:w:f:";
        const struct option long_options[] = {
                {"nop", 0, 0, 0},
                {"pid", 1, 0, 0},
                {"wait", 1, 0, 0},
                {"freeze", 1, 0, 0}
        };

        pid_found = 0;
        wait = 5;
        freeze = 15;
        do {
                opt = getopt_long(argc, argv, short_options, long_options, NULL);
                switch (opt) {
                        case 'n': 
                                break;
                        case 'p':
                                pid_found = 1;
                                pid = atoi(optarg);
                                break;
                        case 'w':
                                wait = atoi(optarg);
                                /*printf("got wait = %d\n", wait);*/
                                break;
                        case 'f':
                                freeze = atoi(optarg);
                                /*printf("got freeze = %d\n", freeze);*/
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -w wait_factor -f freeze_factor -p pid\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        if ( pid_found == 1 ) 
                issue_freeze(pid, wait, freeze);
        return 0;
}

