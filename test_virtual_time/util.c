#include <errno.h>	// for strerror
#include <string.h>
#include <sched.h>	// for unshare
#include <stdio.h> 	// for file operations
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>	// for PATH_MAX

#include "util.h"

/**
 * General helpers for error checking and time calculation
 **/
long check_syscall_status(long ret, char* syscall_name)
{
	if(ret == -1) {
		printf("\n[error] %s fails with error: %s\n",
				syscall_name, strerror(errno));
		exit(errno);
	}
	return ret;
}

int timeval_substract(struct timeval* result,
		struct timeval* x, struct timeval* y)
{
	if (x->tv_usec < y->tv_usec )
	{
		int nsec = (y->tv_usec - x->tv_usec) / USEC_PER_SEC + 1;
		y->tv_usec -= USEC_PER_SEC * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > USEC_PER_SEC)
	{
		int nsec = (x->tv_usec - y->tv_usec) / USEC_PER_SEC + 1;
		y->tv_usec += USEC_PER_SEC * nsec;
		y->tv_sec -= nsec;
	}
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	return x->tv_sec > y->tv_sec;
}

long int timeval_to_usec(struct timeval tv)
{
	return tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}

static const float TDF_MIN = 0.001f;
static const int  TDF_MAX = 100;
static const size_t TDF_STR_LEN = 5;			// 4 digit number

#define _GNU_SOURCE 			// for unshare system call

int virtual_time_unshare(int flags)
{
	int ret;

	ret = unshare(flags | CLONE_NEWTIME);
	return check_syscall_status(ret, "unshare");
}

/*
int set_new_dilation(pid_t pid, float tdf)
{
	FILE *proc_file;
	char path[PATH_MAX];
	int written_count = 0;

	if ( tdf >= TDF_MIN && tdf < TDF_MAX ){
		sprintf(path, "/proc/%d/dilation", pid);
		proc_file = fopen(path, "w");
		if ( !proc_file ) {
			printf("cannot open %s with error %s\n",
					path, strerror(errno));
			return -1;
		}
		printf("About to set tdf to %f\n", tdf);
		// count should equal strlen(tdf_str)
		// FIXME: echo 1000TDF to kernel
		written_count = fprintf(proc_file, "%d", (int)(tdf * 1000));
		fclose(proc_file);
	}
	printf("%d written\n", written_count);

	return written_count;
}
*/

/*
 * more straightforward way is:
	char cmd[100];
	printf(cmd, "echo %d > /proc/%d/dilation", tdf, pid);
	system(cmd);
 */
int set_new_dilation(pid_t pid, float tdf)
{
	int proc_file;
	char path[PATH_MAX];
	char tdf_str[TDF_STR_LEN];
	size_t count;
	ssize_t written_count = 0;

	if ( tdf >= TDF_MIN && tdf < TDF_MAX ){
		sprintf(path, "/proc/%d/dilation", pid);
		proc_file = open(path, O_WRONLY);
		if ( proc_file == -1 ) {
			printf("cannot open %s with error %s\n",
					path, strerror(errno));
			return -1;
		}
		// echo 1000TDF to kernel
		count = sprintf(tdf_str, "%d", (int)(tdf * 1000));
		// count should equal strlen(tdf_str)
		written_count = write(proc_file, tdf_str, count);
		close(proc_file);
	}
	return written_count;
}

int virtual_time_exit(pid_t pid)
{
	return set_new_dilation(pid, 0);
}

/*
 * more straightforward way:
	char cmd[100];
	sprintf(cmd, "cat /proc/%d/%s", pid, field);
	system(cmd);
 */
static int read_proc_field(pid_t pid, char* field)
{
	int proc_file;
	char path[PATH_MAX];
	ssize_t read_count = 0;
	char* result;

	sprintf(path, "/proc/%d/%s", pid, field);
	proc_file = open(path, O_RDONLY);
	if ( proc_file == -1 ) {
		printf("cannot open %s with error %s\n",
				path, strerror(errno));
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

static int write_proc_freeze(pid_t pid, char* val)
{
	int proc_file;
	char path[PATH_MAX];

	ssize_t written_count = 0;

	sprintf(path, "/proc/%d/freeze", pid);
	proc_file = open(path, O_WRONLY);
	if ( proc_file == -1 ) {
		printf("cannot open %s with error %s\n",
				path, strerror(errno));
		return -1;
	}
	written_count = write(proc_file, val, 1);
	close(proc_file);
	return written_count;
}

int freeze_proc(pid_t pid)
{
	char *val = "1";
	return write_proc_freeze(pid, val);
}

int unfreeze_proc(pid_t pid)
{
	char *val = "0";
	return write_proc_freeze(pid, val);
}

int show_proc_dilation(pid_t pid)
{
	return read_proc_field(pid, "dilation");
}

int show_proc_freeze(pid_t pid)
{
	return read_proc_field(pid, "freeze");
}




