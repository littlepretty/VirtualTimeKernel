CC=gcc
INCLUDE=-I. 
CP=cp
PTHREAD=-pthread

all: freeze_all_procs accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation

test: test_accuracy test_accu_freeze test_ovhd_dilaton test_ovhd_set_dilation

.PHONY: clean vtutil.o

# do testing
test_accu_dilation: accu_dilation
	sudo ./accu_dilation.sh

test_freeze_all_procs: freeze_all_procs
	sudo ./freeze_all_procs.sh

test_accu_freeze: accu_freeze
	sudo ./accu_freeze.sh	
	cat accu_freeze.log

test_ovhd_dilation: ovhd_dilation
	sudo ./$< > ovhd_dilation.log
	cat ovhd_dilation.log

test_ovhd_set_dilation: ovhd_set_dilation
	sudo ./$< > ovhd_set_dilation.log
	cat ovhd_set_dilation.log

# compile and link
freeze_all_procs: vtutil.o freeze_all_procs.c accu_freeze freeze_all_procs.sh
	$(CC) $(INCLUDE) $(PTHREAD) freeze_all_procs.c $< -o $@

accu_dilation: vtutil.o accu_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) accu_dilation.c $< -o $@

accu_freeze: vtutil.o freeze_me.c freeze_other.c accu_freeze.sh
	$(CC) $(INCLUDE) $(PTHREAD) freeze_me.c $< -o freeze_me 
	$(CC) $(INCLUDE) $(PTHREAD) freeze_other.c $< -o freeze_other 

ovhd_dilation: vtutil.o ovhd_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) ovhd_dilation.c $< -o $@ 

ovhd_set_dilation: vtutil.o ovhd_set_dilation.c
	$(CC) $(INCLUDE) $(PTHREAD) ovhd_set_dilation.c $< -o $@ 

vtutil.o: vtutil.c
	$(CC) $(INCLUDE) -D SHOW_OVHD -c $<
	cp -v vtutil.c vtutil.h ../mininet/

load_vt_module:
	sudo insmod ../virtual_time_module.ko

rm_vt_module:
	sudo rmmod virtual_time_module

clean:
	rm freeze_all_procs accu_dilation freeze_me freeze_other ovhd_dilation ovhd_set_dilation *.o

