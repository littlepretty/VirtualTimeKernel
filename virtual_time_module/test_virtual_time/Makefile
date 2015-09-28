CC=gcc
INCLUDE=-I. 
CP=cp

all: accu_gtod ovhd_gtod ovhd_settdf

accu_gtod: util
	$(CC) $(INCLUDE) -o accu_gtod accu_gtod.c util.o

ovhd_gtod: util
	$(CC) $(INCLUDE) -o ovhd_gtod ovhd_gtod.c util.o

ovhd_settdf: util 
	$(CC) $(INCLUDE) -o ovhd_settdf ovhd_settdf.c util.o

util: util.c
	$(CC) $(INCLUDE) -c util.c

load_vt_module:
	sudo insmod ../virtual_time_module.ko
rm_vt_module:
	sudo rmmod virtual_time_module

.PHONY: clean
clean:
	rm test_syscalls accu_gtod ovhd_gtod ovhd_settdf *.o
