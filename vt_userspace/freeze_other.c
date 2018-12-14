#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "vtutil.h"

pid_t pid;
int freeze_duration;

void sigint_handler(int signum)
{
        printf("Exit freeze daemon\n");
        exit(0);
}

void issue_freeze(int signum)
{
        if (pid != -1) {
                printf("[freeze other] freeze %d\n", pid);
                freeze_proc(pid);
                usleep(freeze_duration);
                unfreeze_proc(pid);
                printf("[freeze other] unfreeze %d\n", pid);
        }
}

void get_pid_from_fifo()
{
        int server_fd;
        ssize_t num_read;
        char pid_str[PID_MAX_LEN];

        umask(0);
        mkfifo(FREEZE_FIFO, S_IRUSR | S_IWUSR | S_IWGRP);
        server_fd = open(FREEZE_FIFO, O_RDONLY); /* block until freeze_me also open */
        num_read = read(server_fd, pid_str, PID_MAX_LEN);
        if (num_read > 0) {
                pid = atoi(pid_str);
                printf("[freeze other] got pid %d from %s\n", pid, pid_str);
        }
        while (1) {
                /* wait for signal from freeze_me */
        }
}

int main(int argc, char** argv)
{
        int opt, index, i;
        const char* const short_options = "f:";

        pid = -1;
        freeze_duration = 10000; // 10ms
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'f':
                                freeze_duration = atoi(optarg);
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -f value(microsecs) \n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        /*printf("gonna freeze process[%d] %d seconds\n", pid, freeze);*/
        /*printf("but wait %d microseconds\n", wait);*/

        signal(SIGINT, sigint_handler);
        if (signal(SIGUSR1, issue_freeze) == SIG_ERR) {
                printf("[freeze other] install SIGUSR1 handler fail\n");
        }
        get_pid_from_fifo();

        return 0;
}
