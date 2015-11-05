#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

#include "vtutil.h"

extern pipe_fd[2];

void sigint_handler(int dummy)
{
        close(pipe_fd[0]);
}

void issue_freeze(int pause)
{
        char pid_str[PID_MAX_LEN];
        pid_t pid;
        ssize_t num_read = 0;

        while (1) {
                close(pipe_fd[1]);
                num_read = read(pipe_fd[0], pid_str, PID_MAX_LEN);
                if (num_read > 0) {
                        pid = atoi(pid_str);
                        printf("to freeze %d\n", pid);
                        freeze_proc(pid);
                        usleep(pause);
                        unfreeze_proc(pid);
                }
        }
}

int main(int argc, char** argv)
{
        int opt, index, i;
        int freeze; /* freeze @freeze seconds */  
        const char* const short_options = "f:";

        freeze = 10000; // 10ms
        do {
                opt = getopt(argc, argv, short_options);
                switch (opt) {
                        case 'f':
                                freeze = atoi(optarg);
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -f freeze(microsecs) \n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        /*printf("gonna freeze process[%d] %d seconds\n", pid, freeze);*/
        /*printf("but wait %d microseconds\n", wait);*/

        signal(SIGINT, sigint_handler);
        issue_freeze(freeze);
        
        return 0;
}

