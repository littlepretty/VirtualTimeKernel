#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>		// for getopt()
#include <time.h>		// for timeval
#include <unistd.h>		// for getpid()
#include <sys/types.h>

#include "vtutil.h"

int elapsed[NR_ACCU_ROUND];
int dilated_elapsed[NR_ACCU_ROUND];

void fill_elapsed()
{
        struct timeval prev;
        struct timeval next;
        struct timeval diff;
        int ret;
        long int i, j;
        long int usec;

        for (i = 0; i < NR_ACCU_ROUND; ++i)
        {
                ret = gettimeofday(&prev, NULL);
                /*check_syscall_status(ret, "gettimeofday");*/
                for (j = 0; j < CNT_SLEEP; ++j) {
                        // do nothing
                }
                ret = gettimeofday(&next, NULL);
                /*check_syscall_status(ret, "gettimeofday");*/
                ret = timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                elapsed[i] = usec;
        }
}

void fill_dilated_elapsed(int dil)
{
        struct timeval prev, next, diff;
        int ret;
        long int usec;
        pid_t pid = getpid();

        ret = unshare(CLONE_NEWNET | CLONE_NEWNS);
        check_syscall_status(ret, "unshare");
        ret = set_new_dilation(pid, dil);
        show_proc_dilation(pid);

        long int i, j;
        for (i = 0; i < NR_ACCU_ROUND; ++i) {
                ret = gettimeofday(&prev, NULL);
                /*check_syscall_status(ret, "gettimeofday");*/
                for (j = 0; j < CNT_SLEEP; ++j) {
                        // do nothing
                }
                ret = gettimeofday(&next, NULL);
                /*check_syscall_status(ret, "gettimeofday");*/
                ret = timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                dilated_elapsed[i] = usec;
        }
        ret = virtual_time_exit(pid);
}

void actual_dilation(int dil)
{
        float q;
        int i;
        int count = 0;
        for (i = 0; i < NR_ACCU_ROUND; ++i) {
                q = (float)elapsed[i] / (float)dilated_elapsed[i];
                if ((q - dil)*(q - dil) > 1) {
                        printf("[error] round %d: %f\n", i, q);
                        ++count;
                } else {
                        /*printf("[good] round %d: %f\n", i, q);*/
                }
        }
        printf("[summary] %d bad dilations\n", count);
}

const char* program_name;

int main(int argc, char* const argv[])
{
        const char* const short_options = "t:edp";
        const struct option long_options[] = {
                { "tdf", 1, NULL, 't'},
                { "elapsed", 0, NULL, 'e'},
                { "dilated", 0, NULL, 'd'},
                { "print", 0, NULL, 'p'},
                { NULL, 0, NULL, 0}
        };

        int next_option;
        int run_elapsed = 0;
        int run_dilated = 0;
        int print_dil = 0;
        int dilation = 1;

        program_name = argv[0];

        do {
                next_option = getopt_long(argc, argv, short_options, long_options, NULL);
                switch(next_option) {
                        case 't':
                                dilation = atoi(optarg);
                                break;
                        case 'e':
                                run_elapsed = 1;
                                break;
                        case 'd':
                                // if (dilation == 1) {
                                //     printf("[error] run dilated program should feed non 1 TDF\n");
                                //     exit(1);
                                // }
                                run_dilated = 1;
                                break;
                        case 'p':
                                if (run_elapsed == 1 && run_dilated == 1) {
                                        print_dil = 1;
                                }
                                break;
                        case -1:
                                /*printf("invalid input parameters\n");*/
                                break;
                        default:
                                printf("abort\n");
                                abort();
                                break;
                }
        } while(next_option != -1);
        
        if (run_elapsed) {
                fill_elapsed();
        }
        
        printf("Run fill_elapsed() parameters\n"); 
        
        if (run_dilated) {
                fill_dilated_elapsed(dilation);
        }
        
        printf("Run fill_dilated_elapsed() parameters\n"); 
        
        if (print_dil) {
                actual_dilation(dilation);
        }

        return 0;
}


