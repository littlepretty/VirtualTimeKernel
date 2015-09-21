#include "virtual_time_module.h"

MODULE_LICENSE("GPL");

int vt_module_init(void)
{
	printk(KERN_INFO "Virtual time module is loaded.\n");
	return 0;
}

void vt_module_cleanup(void)
{
	printk(KERN_INFO "Virtual time module is unloaded.\n");
	return;
}

#ifndef __NR_set_tdf
#define __NR_set_tdf 317
#endif
/*
 * ppid == 0 : change caller itself's dilation
 * ppid != 0 : change caller's parent's dilation
 */
int change_tdf(int dilation, int ppid)
{
	return set_tdf(dilation, ppid);
}

void freeze(int* pgid_list, size_t length)
{
	int i;
	struct task_struct* group_leader;
	struct timespec ts;
	s64 now;
	
	if ( pgid_list && length > 0 ) {
		for ( i = 0; i < length; ++i ) {
			rcu_read_lock();
			// group leader's pgid == pid
			group_leader = pid_task(find_vpid(pgid_list[i]), PIDTYPE_PID); // or "find_pid_ns()"?
			if ( group_leader ) {
				__getnstimeofday(&ts);
				now = timespec_to_ns(&ts);
				group_leader->freeze_start_nsec = now;
				rcu_read_unlock();
				kill_pgrp(task_pgrp(group_leader), SIGSTOP, 1); // or "task_pid()"?
			} else {
				rcu_read_unlock();
			}
			
		}
	}
}

void unfreeze(int* pgid_list, size_t length)
{
	int i;
	struct task_struct* group_leader;
	struct timespec ts;
	s64 now;

	if ( pgid_list && length > 0 ) {
		for ( i = 0; i < length; ++i ) {
			rcu_read_lock();
			group_leader = pid_task(find_vpid(pgid_list[i]), PIDTYPE_PID);	
			if ( group_leader ) {
				__getnstimeofday(&ts);
				now = timespec_to_ns(&ts);
				group_leader->freeze_past_nsec += now - group_leader->freeze_start_nsec;
				group_leader->freeze_start_nsec = 0;
				rcu_read_unlock();
				kill_pgrp(task_pgrp(group_leader), SIGCONT, 1);
			} else {
				rcu_read_unlock();
			}
		}
	}
}

module_init(vt_module_init);
module_exit(vt_module_cleanup);


