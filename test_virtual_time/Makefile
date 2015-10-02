CC=gcc
INCLUDE=-I. 
CP=cp

all: test accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation

test: test_accuracy test_freeze_accuracy test_ovhd_dilaton test_ovhd_set_dilation

# do testing
test_dilation_accuracy: accu_dilation
	sudo ./$@ -t 2 -e -d -p > dilation_accuracy.log
	sudo ./$@ -t 4 -e -d -p >> dilation_accuracy.log
	sudo ./$@ -t 8 -e -d -p >> dilation_accuracy.log
	sudo ./$@ -t 16 -e -d -p >> dilation_accuracy.log
	cat dilation_accuracy.log

test_freeze_accuracy: accu_freeze
	sudo ./freeze_driver.sh	

test_ovhd_dilation: ovhd_dilation
	sudo ./$@ > dilation_ovhd.log
	cat dilation_ovhd.log

test_ovhd_set_dilation: ovhd_set_dilation
	sudo ./$@ > set_dilation_ovhd.log
	cat set_dilation_ovhd.log

# compile and link
accu_dilation: util
	$(CC) $(INCLUDE) -o test_dilation_accuracy accu_dilation.c $<.o

accu_freeze: util
	$(CC) $(INCLUDE) -o run_long_loop time_long_loop.c $<.o
	$(CC) $(INCLUDE) -o issue_freeze freeze_other.c $<.o

ovhd_dilation: util
	$(CC) $(INCLUDE) -o test_ovhd_dilation ovhd_dilation.c $<.o

ovhd_set_dilation: util 
	$(CC) $(INCLUDE) -o test_ovhd_set_dilation ovhd_set_dilation.c $<.o

util: util.c
	$(CC) $(INCLUDE) -c util.c

load_vt_module:
	sudo insmod ../virtual_time_module.ko
rm_vt_module:
	sudo rmmod virtual_time_module

.PHONY: clean
clean:
	rm accu_dilation accu_freeze ovhd_dilation ovhd_set_dilation *.o


