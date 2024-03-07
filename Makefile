
KERN_DIR = /home/stoicus/imx6ull/bsp/100ask_imx6ull-sdk/Linux-4.9.88

obj-m	+= gpio_drv.o

all:
	make -C $(KERN_DIR) M=`pwd` modules 
	# $(CROSS_COMPILE)gcc -o gpio_test gpio_test.c 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
	rm -f gpio_test

