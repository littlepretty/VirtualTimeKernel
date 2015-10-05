#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()

#include "util.h"

void run_long_loop(int vt_opt)
{
        struct timeval prev, next, diff, tmp;
        int ret;
        long int i, usec;

        if (vt_opt) {
                virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);
        }

        pid_t pid = getpid();
        printf("****** Process[%d] to be freezed ******\n", pid); 
        gettimeofday(&prev, NULL);
        for ( i = 0; i < 10 * NR_NOP_ROUND; ++i ) {
                // do nothing
        }
        gettimeofday(&next, NULL);
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("I elapsed %ld micro_sec total\n", usec);
}

int main(int argc, char* argv[])
{
        const char* short_options = "v";

        int opt;
        int virtual_time_flag;

        virtual_time_flag = 0;
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'v':
                                virtual_time_flag = 1;
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s [-v]\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        run_long_loop(virtual_time_flag);

        return 0;
}
