#include <linux/types.h>
#include <linux/sched.h>
#include <limits.h>

#define NR_GTOD_ROUND 10000000UL
#define NR_SET_ROUND 1000000UL
#define MOUNT_SYS_MIN_VERSION "2.6.35"
#define USEC_PER_SEC 1000000UL

#define NR_ACCU_ROUND 100UL
#define CNT_SLEEP 10000UL

#define CLONE_NEWTIME 0x02000000

#define NSCLONEFLGS		\
(				\
        SIGCHLD         |	\
        CLONE_NEWNS     |	\
        CLONE_NEWUTS    |	\
        CLONE_NEWIPC    |	\
        CLONE_NEWPID    |	\
        CLONE_NEWNET		\
)

#define PID_MAX_LEN 8 
#define FREEZE_FIFO "/tmp/freeze_fifo" /* named pipe key in freeze test */

#define TDF_MIN 0.001f
#define TDF_MAX 100
#define TDF_STR_LEN 8 /* 4 digit number * 1000 */

/* passing necessary data to pthreads */
struct dilation_data {
        pid_t pid;
        char* tdf_str;
};

// general helpers
int check_syscall_status(int ret, char* sys_cl_nm);
long int timeval_to_usec(struct timeval tv);
int timeval_substract(struct timeval* result, struct timeval* x, struct timeval* y);

/* dilation helpers */
int virtual_time_unshare(int flags); // for historical reason
int set_new_dilation(pid_t pid, float tdf); // NOTICE: 0.001 <= tdf < 100
int virtual_time_exit(pid_t pid);
void* dilation_work(void *p);

/* freeze helpers */
int freeze_proc(pid_t pid);
int unfreeze_proc(pid_t pid);
void* freeze_work(void *p);
void* unfreeze_work(void *p);


void freeze_all_procs(pid_t* pid_list, size_t size);
void unfreeze_all_procs(pid_t* pid_list, size_t size);

/* show helpers */
int show_proc_freeze(pid_t pid);
int show_proc_dilation(pid_t pid);

void kickoff_pthreads_dilate(pid_t* pid_list, size_t size, char *tdf_str, void *(* func)(void *));
void kickoff_pthreads_freeze(pid_t* pid_list, size_t size, void *(*func)(void *), char* action);
