#include <stdlib.h>
#include <stdio.h>
#include <time.h>               // for timeval
#include <unistd.h>             // for getpid()
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include "vtutil.h"

struct timeval prev, next, diff, temp;

void freeze_me_loop(pid_t server_pid, int pause_interval, int repetition)
{
        long int usec, i;
        pid_t pid;
        char pid_str[PID_MAX_LEN];
        int server_fd;
        struct timespec wait_ts = {.tv_sec = 0, .tv_nsec = 100};
        struct timespec rem;

        virtual_time_unshare(CLONE_NEWNET | CLONE_NEWNS);

        pid = getpid();
        sprintf(pid_str, "%d", pid);
        umask(0);
        server_fd = open(FREEZE_FIFO, O_WRONLY);
        write(server_fd, pid_str, strlen(pid_str)); /* write my pid */

        for (i = 0; i < repetition; ++i) {
                usleep(pause_interval);

                gettimeofday(&prev, NULL);
                printf("[freeze_me] GTOD 1\n");

                kill(server_pid, SIGUSR1);
                nanosleep(&wait_ts, &rem);

                printf("[freeze_me] GTOD 2\n");
                gettimeofday(&next, NULL);

                timeval_substract(&diff, &next, &prev);
                usec = timeval_to_usec(diff);

                printf("%ld\n", usec); /* log error */
        }
}

/*
 * To run this program, use 'setsid time_long_loop -v -d duration'
 * so that this program detached from its parent(your shell/tty) and
 * freeze this process will not stop its parent or brothers
 */
int main(int argc, char* argv[])
{
        const char* short_options = "s:r:i:";
        int opt, repetition, interval, server_pid;

        repetition = 0;
        interval = 1000;
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 's':
                                server_pid = atoi(optarg);
                        case 'r':
                                repetition = atoi(optarg);
                                break;
                        case 'i':
                                interval = atoi(optarg);
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -i interval -r repeats\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        freeze_me_loop(server_pid, interval, repetition);

        return 0;
}
