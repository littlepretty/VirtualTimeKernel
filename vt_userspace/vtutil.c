#include <errno.h>	// for strerror
#include <string.h>
#include <sched.h>	// for unshare
#include <stdio.h> 	// for file operations
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>	// for PATH_MAX
#include <pthread.h>    // for pthread

#include "vtutil.h"

#define _GNU_SOURCE // for unshare system call

/**
 * General helpers for error checking and time calculation
 **/
int check_syscall_status(int ret, char* syscall_name)
{
        if(ret != 0) {
                printf("[error %d] %s fails with error: %s\n",
                                ret, syscall_name, strerror(errno));
                exit(errno);
        }
        return ret;
}

int timeval_substract(struct timeval* result, struct timeval* x, struct timeval* y)
{
        if (x->tv_usec < y->tv_usec ) {
                int nsec = (y->tv_usec - x->tv_usec) / USEC_PER_SEC + 1;
                y->tv_usec -= USEC_PER_SEC * nsec;
                y->tv_sec += nsec;
        }
        if (x->tv_usec - y->tv_usec > USEC_PER_SEC) {
                int nsec = (x->tv_usec - y->tv_usec) / USEC_PER_SEC + 1;
                y->tv_usec += USEC_PER_SEC * nsec;
                y->tv_sec -= nsec;
        }
        result->tv_sec = x->tv_sec - y->tv_sec;
        result->tv_usec = x->tv_usec - y->tv_usec;

        return x->tv_sec > y->tv_sec;
}

long timeval_to_usec(struct timeval tv)
{
        return tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}

/**
 * more straightforward way:
 * char cmd[100];
 * sprintf(cmd, "cat /proc/%d/%s", pid, field);
 * system(cmd);
 */
int read_proc_field(pid_t pid, char* field)
{
        int proc_file;
        char path[PATH_MAX];
        ssize_t read_count = 0;
        char* result;

        sprintf(path, "/proc/%d/%s", pid, field);
        proc_file = open(path, O_RDONLY);
        if ( proc_file == -1 ) {
                fprintf(stderr, "read %s failed with error: cannot open %s because %s\n", field, path, strerror(errno));
                return -1;
        }
        result = malloc(sizeof(char) * TDF_MAX);
        if ( result == NULL ) {
                return -1;
        }
        read_count = read(proc_file, result, TDF_MAX);
        close(proc_file);
        printf("%s: %s\n", path, result);
        return read_count;
}

/**
 * more straightforward way:
 * char cmd[100];
 * sprintf(cmd, "echo %s > /proc/%d/%s", val, pid, field);
 * system(cmd);
 */
int write_proc_field(pid_t pid, char* field, char* val)
{
        int proc_file;
        char path[PATH_MAX];
        ssize_t written_count = 0;
        size_t val_len = strlen(val);

        if (val_len <= 0) {
                fprintf(stderr, "cannot write null str\n");
                return -1;
        }

        sprintf(path, "/proc/%d/%s", pid, field);
        /*printf("echo %s > %s\n", val, path);*/
        proc_file = open(path, O_WRONLY);
        if (proc_file == -1) {
                fprintf(stderr, "cannot open %s because %s\n", path, strerror(errno));
                return -1;
        }
        written_count = write(proc_file, val, val_len);
        if (written_count == 0) {
                fprintf(stderr, "nothing is written to %s\n", path);
                return -1;
        } else if (written_count < 0) {
                fprintf(stderr, "write %s fails with error '%s'", path, strerror(errno));
                return -1;
        } else {
                return written_count;
        }
}

int virtual_time_unshare(int flags)
{
        int ret;

        ret = unshare(flags | CLONE_NEWTIME);
        return check_syscall_status(ret, "unshare");
}

/**
 * more straightforward way is:
 * char cmd[100];
 * sprintf(cmd, "echo %d > /proc/%d/dilation", tdf, pid);
 * system(cmd);
 */
int set_new_dilation(pid_t pid, float tdf)
{
        char tdf_str[TDF_STR_LEN];
        size_t count;
        ssize_t written_count = 0;
#ifdef SHOW_OVHD
        struct timeval prev, next, ovhd;
        long int usec;

        gettimeofday(&prev, NULL);
#endif
        if (tdf >= TDF_MIN && tdf < TDF_MAX){
                /* echo 1000*TDF to kernel */
                count = sprintf(tdf_str, "%d", (int)(tdf * 1000));
                /* count = strlen(tdf_str) */
                written_count = write_proc_field(pid, "dilation", tdf_str); //write(proc_file, tdf_str, count);
        }
#ifdef SHOW_OVHD
        gettimeofday(&next, NULL);
        timeval_substract(&ovhd, &next, &prev);
        usec = timeval_to_usec(ovhd);
        printf("[set dilation ovhd = %ld]\n", usec);
#endif
        return written_count;
}

int virtual_time_exit(pid_t pid)
{
        return set_new_dilation(pid, 0);
}

/**
 * deprecated interface to freeze a process
 */
/*
int write_proc_freeze(pid_t pid, char* val)
{
        write_proc_field(pid, "freeze", val);

        int proc_file;
        char path[PATH_MAX];
        ssize_t written_count = 0;

        sprintf(path, "/proc/%d/freeze", pid);
        proc_file = open(path, O_WRONLY);
        if (proc_file == -1) {
                fprintf(stderr, "freeze(%s) failed with error: cannot open %s because %s\n", val, path, strerror(errno));
                return -1;
        }
        written_count = write(proc_file, val, 1);
        close(proc_file);
        printf("%d written\n", written_count);
        return written_count;
}
*/

int freeze_proc(pid_t pid)
{
        char *val = "1";
#ifdef SHOW_OVHD
        struct timeval prev, next, ovhd;
        long int usec;
        int result;

        gettimeofday(&prev, NULL);
        result = write_proc_field(pid, "freeze", val); //write_proc_freeze(pid, val);
        gettimeofday(&next, NULL);
        timeval_substract(&ovhd, &next, &prev);
        usec = timeval_to_usec(ovhd);
        printf("[freeze proc %d] takes %ld]\n", pid, usec);

        return result;
#else
        return write_proc_field(pid, "freeze", val); //write_proc_freeze(pid, val);
#endif
}

int unfreeze_proc(pid_t pid)
{
        char *val = "0";
#ifdef SHOW_OVHD
        struct timeval prev, next, ovhd;
        long int usec;
        int result;

        gettimeofday(&prev, NULL);
        result = write_proc_field(pid, "freeze", val); //write_proc_freeze(pid, val);
        gettimeofday(&next, NULL);
        timeval_substract(&ovhd, &next, &prev);
        usec = timeval_to_usec(ovhd);
        printf("[unfreeze proc %d] takes %ld]\n", pid, usec);

        return result;
#else
        return write_proc_field(pid, "freeze", val); //write_proc_freeze(pid, val);
#endif
}

int show_proc_dilation(pid_t pid)
{
        return read_proc_field(pid, "dilation");
}

int show_proc_freeze(pid_t pid)
{
        return read_proc_field(pid, "freeze");
}

/*
 * pthread's routines for dilation and freeze
 */
void* dilation_work(void *p)
{
        struct dilation_data *data = (struct dilation_data *)p;
        write_proc_field(data->pid, "dilation", data->tdf_str);
        pthread_exit((void *)data);
}

void* freeze_work(void *p)
{
        char val[] = "1";
        pid_t* pid = (pid_t *)p;
        write_proc_field(*pid, "freeze", val);
        pthread_exit((void *)pid);
}

void* unfreeze_work(void *p)
{
        char val[] = "0";
        pid_t* pid = (pid_t *)p;
        write_proc_field(*pid, "freeze", val);
        pthread_exit((void *)pid);
}

void kickoff_pthreads_dilate(pid_t* pid_list, size_t size, char *tdf, void *(* func)(void *))
{
        pthread_t* threads;
        pthread_attr_t attr;
        int i, rc;
        void *status;

        threads = malloc(sizeof(pthread_t) * size);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        for (i = 0; i < size; ++i) {
                struct dilation_data data = {.pid = pid_list[i], .tdf_str = tdf};
                if (pthread_create(&threads[i], NULL, func, (void *)(&data)) != 0) {
                        fprintf(stderr, "create pthread failed\n");
                        exit(-1);
                }
        }
        pthread_attr_destroy(&attr);
        for (i = 0; i < size; ++i) {
                rc = pthread_join(threads[i], &status);
                if (rc) {
                        fprintf(stderr, "join thread fail with %d\n", rc);
                        exit(-1);
                }
        }
}

void kickoff_pthreads_freeze(pid_t* pid_list, size_t size, void *(* func)(void *), char *action)
{
        pthread_t* threads;
        pthread_attr_t attr;
        int i, rc;
        void *status;
#ifdef SHOW_OVHD
        struct timeval start, end, ovhd;
        long usec;

        gettimeofday(&start, NULL);
#endif
        threads = malloc(sizeof(pthread_t) * size);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        for (i = 0; i < size; ++i) {
                if (pthread_create(&threads[i], NULL, func, (void *)(&pid_list[i])) != 0) {
                        fprintf(stderr, "create pthreads failed\n");
                }
        }
        pthread_attr_destroy(&attr);
        for (i = 0; i < size; ++i) {
                rc = pthread_join(threads[i], &status);
                if (rc) {
                        fprintf(stderr, "join pthread fail with %d\n", rc);
                        exit(-1);
                }
                /*printf("%s thread[%d] joined for pid[%d]\n", action, i, *((pid_t*)status));*/
        }
#ifdef SHOW_OVHD
        gettimeofday(&end, NULL);
        timeval_substract(&ovhd, &end, &start);
        usec = timeval_to_usec(ovhd);
        if (strcmp(action, "freeze") == 0) {
                printf("freeze %ld\n", usec);
        } else if (strcmp(action, "unfreeze") == 0) {
                printf("unfreeze %ld\n", usec);
        } else {
                printf("unknown action %s\n", action);
        }
#endif
        /* should issue exit on main thread if we do join() */
        /*pthread_exit(NULL);*/
}
