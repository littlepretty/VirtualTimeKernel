#include <linux/types.h>
#include <linux/sched.h>
#include <limits.h>

#define NR_GTOD_ROUND 10000000UL
#define NR_SET_ROUND 1000000UL
#define MOUNT_SYS_MIN_VERSION "2.6.35"
#define USEC_PER_SEC 1000000UL

#define NR_ACCU_ROUND 10000UL
#define CNT_SLEEP 10000000UL

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

// general helpers
long check_syscall_status(long ret, char* sys_cl_nm);
long int timeval_to_usec(struct timeval tv);
int timeval_substract(struct timeval* result, struct timeval* x, struct timeval* y);

// dilation helpers
int virtual_time_unshare(int flags); // for historical reason
int set_new_dilation(pid_t pid, float tdf); // NOTICE: 0.001 <= tdf < 100
int virtual_time_exit(pid_t pid);

// freeze helpers
int freeze_proc(pid_t pid);
int unfreeze_proc(pid_t pid);

int freeze_all_procs(pid_t* pid_list, size_t size);
int unfreeze_all_procs(pid_t* pid_list, size_t size);

