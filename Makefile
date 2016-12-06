ifneq ($(KERNELRELEASE),) 
   	obj-m:=hello_timer.o
else 
	KERNELDIR:=/lib/modules/$(shell uname -r)/build 

PWD:=$(shell pwd)

all: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif 
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
