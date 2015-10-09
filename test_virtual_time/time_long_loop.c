#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()

#include "vtutil.h"

void run_long_loop(int vt_opt, int duration_factor)
{
        struct timeval prev, next, diff, tmp;
        int ret;
        long int i, usec;

        if (vt_opt) {
                virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);
        }

        pid_t pid = getpid();
        setsid();
        /*printf("****** Process[%d] to be freezed ******\n", pid); */
        gettimeofday(&prev, NULL);
        for ( i = 0; i < duration_factor * CNT_SLEEP; ++i ) {
                // do nothing
        }
        gettimeofday(&next, NULL);
        ret = timeval_substract(&diff, &next, &prev);
        usec = timeval_to_usec(diff);
        printf("%ld\t", usec);
}

int main(int argc, char* argv[])
{
        const char* short_options = "vd:";
        int opt, virtual_time_flag, duration_factor;

        virtual_time_flag = 0;
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'v':
                                virtual_time_flag = 1;
                                break;
                        case 'd':
                                duration_factor = atoi(optarg);
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s [-v]\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        run_long_loop(virtual_time_flag, duration_factor);

        return 0;
}
