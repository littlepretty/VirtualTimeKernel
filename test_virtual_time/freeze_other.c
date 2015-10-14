#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "vtutil.h"

void issue_freeze(pid_t pid, int wait, int pause)
{
        usleep(wait);
        freeze_proc(pid);
        /*show_proc_dilation(pid);*/
        /*show_proc_freeze(pid);*/
        usleep(pause);
        unfreeze_proc(pid);
        /*show_proc_freeze(pid);*/
        /*printf("%d\t", pause);*/
}

int main(int argc, char** argv)
{
        int pid_found, opt, index, i;
        int wait, freeze; // wait @wait seconds and then freeze @freeze seconds
        int pid;
        const char* const short_options = "np:w:f:";
        const struct option long_options[] = {
                {"nop", 0, 0, 0},
                {"pid", 1, 0, 0},
                {"wait", 1, 0, 0},
                {"freeze", 1, 0, 0}
        };

        /*
         * default values
         */
        pid_found = 0;
        wait = 100; // 100us
        freeze = 10000; // 10ms
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
                                break;
                        case 'f':
                                freeze = atoi(optarg);
                                break;
                        case -1:
                                break;
                        default:
                                printf("Usage: %s -w wait_microsecs -f freeze_microsecs -p pid\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);

        if ( pid_found == 1 ) {
                /*printf("gonna freeze process[%d] %d seconds\n", pid, freeze);*/
                /*printf("but wait %d microseconds\n", wait);*/
                issue_freeze(pid, wait, freeze);
        }
        return 0;
}

