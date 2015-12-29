#!/bin/bash

if [ "z$1" = "z" ]; then
    echo "usage: $0 path/to/kernel/src"
    echo "  e.g. $0 /home/yjq/ExtendKernel/linux-3.16.3/linux-3.16.3"
    exit 1
fi

DST=$1

if [ ! -e $DST ]; then
    echo "error: $DST not found"
    exit 1
fi

# generate patch file
PATCH=VirtualTime.patch
echo "Step 0. patch file written to ${PATCH}"
diff -rup $DST ./ > ${PATCH}
echo ""

FILES="kernel/fork.c		\
kernel/time.c			\
kernel/time/timekeeping.c	\
arch/x86/vdso/vclock_gettime.c	\
include/uapi/linux/sched.h	\
include/linux/sched.h		\
include/linux/init_task.h	\
include/linux/time.h		\
net/sched/sch_htb.c             \
fs/proc/base.c			\
Makefile			\
build_all.sh"

#include/net/sch_generic.h	\
#net_dilation/sched/sch_generic.c       \
#net_dilation/sched/sch_htb.c   \
#net_dilation/core/dev.c        \

# if [ ! -e $DST/virtual_time ]; then
#     mkdir $DST/virtual_time
# fi

echo "Step 1. transfer modified kernel source files"
for f in $FILES; do
    cp -v $f $DST/$f
done

echo ""

#echo "Step 2. install syscall wrapper"
#cp -v syscall_wrapper/* ../test_vt_kernel/syscall.wrap/
#cp -v syscall_wrapper/* ../mininet/
# no need to wrap applications like iperf3

