CC = gcc
INCLUDE = -I. 
CP = cp -v
PTHREAD = -pthread
CFLAGS += $(INCLUDE) $(PTHREAD)
INSTALL_PATH=/usr/local/bin

.PHONY: clean vtutil.o

install: freeze_all_procs dilate_all_procs
	$(CP) freeze_all_procs $(INSTALL_PATH)
	$(CP) dilate_all_procs $(INSTALL_PATH)

# compile and link
all: freeze_listen freeze_all_procs dilate_all_procs accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation
freeze_all_procs: vtutil.o freeze_all_procs.c accu_freeze freeze_all_procs.sh
	$(CC) $(CFLAGS) freeze_all_procs.c $< -o $@

dilate_all_procs: vtutil.o dilate_all_procs.c
	$(CC) $(CFLAGS) dilate_all_procs.c $< -o $@

freeze_listen: vtutil.o freeze_listen.c
	$(CC) $(CFLAGS) freeze_listen.c $< -o $@

accu_dilation: vtutil.o accu_dilation.c
	$(CC) $(CFLAGS) accu_dilation.c $< -o $@

accu_freeze: vtutil.o freeze_me.c freeze_other.c accu_freeze.sh
	$(CC) $(CFLAGS) freeze_me.c $< -o freeze_me 
	$(CC) $(CFLAGS) freeze_other.c $< -o freeze_other 

ovhd_dilation: vtutil.o ovhd_dilation.c
	$(CC) $(CFLAGS) ovhd_dilation.c $< -o $@ 

ovhd_set_dilation: vtutil.o ovhd_set_dilation.c
	$(CC) $(CFLAGS) ovhd_set_dilation.c $< -o $@ 

vtutil.o: vtutil.c
	$(CC) $(INCLUDE) -c $<
	$(CP) vtutil.c vtutil.h ../mininet/

load_vt_module:
	sudo insmod ../virtual_time_module.ko

rm_vt_module:
	sudo rmmod virtual_time_module

clean:
	rm freeze_all_procs accu_dilation freeze_me freeze_other ovhd_dilation ovhd_set_dilation *.o

# do testing
test: test_accu_dilation test_freeze_all_procs test_accu_freeze test_ovhd_dilaton test_ovhd_set_dilation
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

