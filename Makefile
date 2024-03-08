
KERN_DIR = /home/stoicus/imx6ull/bsp/100ask_imx6ull-sdk/Linux-4.9.88
INCDIR := /home/stoicus/imx6ull/source/tem_con/get_data
TARGET := tem_getdata
obj-m = $(TARGET).o
EXTRA_CFLAGS += -I$(INCDIR)
$(TARGET)-objs := dht11.o tem_drv.o

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	$(CROSS_COMPILE)gcc -o tem_test tem_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f gpio_test

