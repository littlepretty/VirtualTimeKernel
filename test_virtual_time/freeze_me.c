#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()

#include "vtutil.h"

struct timeval prev, next, diff, temp;

extern int pipe_fd[2];

void freeze_me_loop(int vt_opt, int pause_interval, int repetition)
{
        long int usec, i;
        int pause_flage;
        pid_t pid;
        char pid_str[PID_MAX_LEN];

        if (vt_opt) virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);

        pid = getpid();
        sprintf(pid_str, "%d", pid);
        for (i = 0; i < repetition; ++i) {
                usleep(pause_interval);

                gettimeofday(&prev, NULL);
                
                // write my pid to PIPE[1]

                write(pipe_fd[1], pid_str, PID_MAX_LEN);

                
                /*usleep(duration);*/
                /*for ( i = 0; i < duration * CNT_SLEEP; ++i) {*/
                        /*gettimeofday(&temp, NULL);*/
                /*}*/

                gettimeofday(&next, NULL);

                timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);
                
                printf("%ld\n", usec);
        }
        close(pipe_fd[1]); /* feed reader EOF */
}

/*
 * To run this program, use 'setsid time_long_loop -v -d duration'
 * so that this program detached from its parent(your shell/tty) and
 * freeze this process will not stop its parent or brothers
 */
int main(int argc, char* argv[])
{ 
        const char* short_options = "vr:i:";
        int opt, virtual_time_flag, repetition, interval;

        virtual_time_flag = 0;
        repetition = 0;
        interval = 1000;
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'v':
                                virtual_time_flag = 1;
                                break;
                        case 'r':
                                repetition = atoi(optarg);
                                break;
                        case 'i':
                                interval = atoi(optarg);
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -i interval -r repeats [-v]\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        if (pipe(pipe_fd) == -1) {
                printf("create pipe error\n");
                exit(1);
        }
        freeze_me_loop(virtual_time_flag, interval, repetition);
                
        return 0;
}
