#!/bin/bash

DST=~/linux_kernel_src

if [ -e $DST ]; then
        sudo rm -rf $DST
fi

if [ ! -e linux-3.16.3.tar.gz ]; then
        wget https://www.kernel.org/pub/linux/kernel/v3.0/linux-3.16.3.tar.gz
fi
echo "Step 0. Unpack kernel source"
tar -xzvf linux-3.16.3.tar.gz
mv linux-3.16.3 $DST

# generate patch file
PATCH=VirtualTime.patch
echo "        Patch file written to ${PATCH}"
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
fs/proc/base.c			\
Makefile			\
build_all.sh"
#include/net/sch_generic.h	\
#net_dilation/sched/sch_generic.c       \
#net_dilation/sched/sch_htb.c   \
#net_dilation/core/dev.c        \

echo "Step 1. Transfer modified kernel source"
for f in $FILES; do
    cp -v $f $DST/$f
done

echo "Step 2. Build new kernel"
cd $DST
sudo ./build_all.sh



