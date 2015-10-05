CC=gcc
INCLUDE=-I. 
CP=cp

all: accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation

test: test_accuracy test_accu_freeze test_ovhd_dilaton test_ovhd_set_dilation

# do testing
test_accu_dilation: accu_dilation
	sudo ./$< -t 2 -e -d -p > accu_dilation.log
	sudo ./$< -t 4 -e -d -p >> accu_dilation.log
	sudo ./$< -t 8 -e -d -p >> accu_dilation.log
	sudo ./$< -t 16 -e -d -p >> accu_dilation.log
	cat accu_dilation.log

test_accu_freeze: accu_freeze
	sudo ./freeze_driver.sh	

test_ovhd_dilation: ovhd_dilation
	sudo ./$< > ovhd_dilation.log
	cat ovhd_dilation.log

test_ovhd_set_dilation: ovhd_set_dilation
	sudo ./$< > ovhd_set_dilation.log
	cat ovhd_set_dilation.log

# compile and link
accu_dilation: util
	$(CC) $(INCLUDE) -o $@ accu_dilation.c $<.o

accu_freeze: util
	$(CC) $(INCLUDE) -o time_long_loop time_long_loop.c $<.o
	$(CC) $(INCLUDE) -o freeze_other freeze_other.c $<.o

ovhd_dilation: util
	$(CC) $(INCLUDE) -o ovhd_dilation ovhd_dilation.c $<.o

ovhd_set_dilation: util 
	$(CC) $(INCLUDE) -o ovhd_set_dilation ovhd_set_dilation.c $<.o

util: util.c
	$(CC) $(INCLUDE) -c util.c

load_vt_module:
	sudo insmod ../virtual_time_module.ko
rm_vt_module:
	sudo rmmod virtual_time_module

.PHONY: clean
clean:
	rm accu_dilation time_long_loop freeze_other ovhd_dilation ovhd_set_dilation *.o


