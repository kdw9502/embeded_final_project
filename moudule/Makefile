#Makefile for a basic kernel module

obj-m   := module.o

KDIR    :=/work/achroimx_kernel
PWD     :=$(shell pwd)
APP := module_test
all: driver app


app:
	arm-none-linux-gnueabi-gcc -static -o $(APP) $(APP).c

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules ARCH=arm

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf *.o
	rm -rf module
	rm -rf Module.symvers
	rm -rf .tmp*
	rm -rf .fpga*
	rm -rf modules.order
