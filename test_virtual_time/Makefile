CC=gcc
INCLUDE=-I. 
CP=cp
PTHREAD=-pthread

all: accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation

test: test_accuracy test_accu_freeze test_ovhd_dilaton test_ovhd_set_dilation

# do testing
test_accu_dilation: accu_dilation
	sudo ./$< -t 2 -e -d -p > accu_dilation.log
	sudo ./$< -t 4 -e -d -p >> accu_dilation.log
	sudo ./$< -t 8 -e -d -p >> accu_dilation.log
	sudo ./$< -t 16 -e -d -p >> accu_dilation.log
	cat accu_dilation.log

test_parallel_freeze: freeze_all_procs
	sudo ./parallel_freeze.sh

test_accu_freeze: accu_freeze
	sudo ./accu_freeze.sh	

test_ovhd_dilation: ovhd_dilation
	sudo ./$< > ovhd_dilation.log
	cat ovhd_dilation.log

test_ovhd_set_dilation: ovhd_set_dilation
	sudo ./$< > ovhd_set_dilation.log
	cat ovhd_set_dilation.log

# compile and link
freeze_all_procs: util.o freeze_all_procs.c accu_freeze parallel_freeze.sh
	$(CC) $(INCLUDE) $(PTHREAD) freeze_all_procs.c $< -o $@

accu_dilation: util.o accu_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) accu_dilation.c $< -o $@

accu_freeze: util.o time_long_loop.c freeze_other.c accu_freeze.sh
	$(CC) $(INCLUDE) $(PTHREAD) time_long_loop.c $< -o time_long_loop 
	$(CC) $(INCLUDE) $(PTHREAD) freeze_other.c $< -o freeze_other 

ovhd_dilation: util.o ovhd_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) ovhd_dilation.c $< -o $@ 

ovhd_set_dilation: util.o ovhd_set_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) ovhd_set_dilation.c $< -o $@ 

util.o: util.c
	$(CC) $(INCLUDE) -c util.c

load_vt_module:
	sudo insmod ../virtual_time_module.ko
rm_vt_module:
	sudo rmmod virtual_time_module

.PHONY: clean
clean:
	rm accu_dilation time_long_loop freeze_other ovhd_dilation ovhd_set_dilation *.o


