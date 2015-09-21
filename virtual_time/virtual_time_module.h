#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h> // find_task_by_vpid
#include <linux/pid.h>
#include <linux/time.h> // __getnstimeofday
#include <asm/uaccess.h> // copy_to_user etc.

extern void freeze(int*, size_t);
extern void unfreeze(int*, size_t);
extern int change_tdf(int dilation, int ppid);
