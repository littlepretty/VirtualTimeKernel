CC=gcc
INCLUDE=-I. 
CP=cp

all: test_accuracy accu_gtod ovhd_gtod ovhd_settdf

test_accuracy: accu_gtod
	OUTPUT=accuracy.log
	sudo ./$@ -t 2 -e -d -p > accuracy.log
	#sudo ./$@ -t 4 -e -d -p >> accuracy.log
	#sudo ./$@ -t 8 -e -d -p >> accuracy.log
	#sudo ./$@ -t 16 -e -d -p >> accuracy.log
	cat $(OUTOUT)

test_ovhd_timekeeping: ovhd_gtod
	sudo ./$@

test_ovhd_set_dilation: ovhd_settdf
	sudo ./$@

accu_gtod: util
	$(CC) $(INCLUDE) -o test_accuracy accu_gtod.c $<.o

ovhd_gtod: util
	$(CC) $(INCLUDE) -o test_ovhd_timekeeping ovhd_gtod.c $<.o

ovhd_settdf: util 
	$(CC) $(INCLUDE) -o test_ovhd_set_dilation ovhd_settdf.c $<.o

util: util.c
	$(CC) $(INCLUDE) -c util.c

load_vt_module:
	sudo insmod ../virtual_time_module.ko
rm_vt_module:
	sudo rmmod virtual_time_module

.PHONY: clean
clean:
	rm test_syscalls accu_gtod ovhd_gtod ovhd_settdf *.o
