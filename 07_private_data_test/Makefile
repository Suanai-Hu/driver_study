obj-m += file.o
KDIR:=/home/hhr/ws/RK3568/kernel
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.o *.ko *.mod.o *.mod.c *.symvers *.order