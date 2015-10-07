#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "util.h"

#define MAX_NUM_PIDS 256

int main(int argc, char** argv)
{
        int pid_found, freeze_flag, unfreeze_flag, opt;
        int index, i;
        pid_t pid_list[MAX_NUM_PIDS];
        size_t pid_size;
        char *next;
        const char* const short_options = "fup:";
        const struct option long_options[] = {
                {"freeze", 0, 0, 0},
                {"unfreeze", 0, 0, 0 },
                {"pid", 1, 0, 0}
        };

        pid_size = 0;
        freeze_flag = unfreeze_flag = 0;
        do {
                opt = getopt_long(argc, argv, short_options, long_options, NULL);
                switch (opt) {
                        case 'f':
                                /*printf("get f\n");*/
                                freeze_flag = 1;
                                break;
                        case 'u':
                                /*printf("get u\n");*/
                                unfreeze_flag = 1;
                                break;
                        case 'p':
                                index = optind - 1;
                                while (index < argc) {
                                        next = strdup(argv[index++]);
                                        if (next[0] != '-') {
                                                pid_list[pid_size++] = atoi(next);
                                        } else break;
                                }
                                optind = index - 1;
                                break;
                        case -1:
                                /*printf("parse ends\n");*/
                                break;
                        default:
                                printf("Usage: %s -[f or u] -p pid1 pid2 pid3 ... pid256\n", argv[0]);
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);
        
        if (pid_size > 0) {
                /*for (i = 0; i < pid_size; ++i) {*/
                     /*printf("pid_list[%d] = %d\n", i, pid_list[i]);*/
                /*}*/
                if (freeze_flag) {
                        // issue freeze
                        kickoff_pthreads(pid_list, pid_size, freeze_work, NULL);
                } else if (unfreeze_flag) {
                        // issue unfreeze
                        kickoff_pthreads(pid_list, pid_size, unfreeze_work, NULL);
                } else {
                        printf("Tell me what you wanna do!\n");
                        printf("Usage: %s -[f or u] -p pid1 pid2 pid3 ... pid256\n", argv[0]);
                }
        }
        return 0;
}

