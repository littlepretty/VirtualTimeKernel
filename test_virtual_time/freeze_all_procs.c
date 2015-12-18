#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "vtutil.h"

static int const MAX_NUM_PIDS = 1024;

void print_usage(char *comm)
{
        printf("Usage: %s -[f or u] -p [pids up to %d]\n", comm, MAX_NUM_PIDS);
}

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
                                print_usage(argv[0]);        
                                exit(EXIT_FAILURE);
                }
        } while (opt != -1);
        
        if (pid_size > 0) {
                /*for (i = 0; i < pid_size; ++i) {*/
                     /*printf("pid_list[%d] = %d\n", i, pid_list[i]);*/
                /*}*/
                if (freeze_flag) {
                        // issue freeze
                        kickoff_pthreads(pid_list, pid_size, freeze_work, "freeze");
                } else if (unfreeze_flag) {
                        // issue unfreeze 
                        kickoff_pthreads(pid_list, pid_size, unfreeze_work, "unfreeze");
                } else {
                        print_usage(argv[0]);
                }
        }
        return 0;
}

