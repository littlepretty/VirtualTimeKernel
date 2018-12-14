#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <string.h>

#include "vtutil.h"

static int const MAX_NUM_PIDS = 1024;

void print_usage(char *comm)
{
        printf("Usage: %s -t tdf -p pids...(up to %d)\n", comm, MAX_NUM_PIDS);
}

int main(int argc, char** argv)
{
        int pid_found, opt;
        int index, i;
        pid_t pid_list[MAX_NUM_PIDS];
        char *tdf_str;
        size_t pid_size;
        char *next;
        const char* const short_options = "t:p:";
        const struct option long_options[] = {
                {"time_dilation", 1, 0, 't'},
                {"pid", 1, 0, 'p'},
                {0, 0, 0, 0}
        };

        pid_size = 0;
        while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
                switch (opt) {
                        case 't':
                                tdf_str = strdup(optarg);
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
                        default:
                                print_usage(argv[0]);
                                exit(EXIT_FAILURE);
                }
        }

        if (pid_size > 0) {
                for (i = 0; i < pid_size; ++i) {
                     printf("pid_list[%d] = %d\n", i, pid_list[i]);
                }
                if (tdf_str != NULL) {
                        printf("%s\n", tdf_str);
                        kickoff_pthreads_dilate(pid_list, pid_size, tdf_str, dilation_work);
                        free(tdf_str);
                } else {
                        print_usage(argv[0]);
                }
        }
        return 0;
}
