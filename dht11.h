#ifndef DHT11_H
#define DHT11_H


//PA11

#include "asm-generic/gpio.h"
#include "asm/gpio.h"
#include "asm/io.h"
#include "asm/uaccess.h"
#include "linux/cdev.h"
#include "linux/dmi.h"
#include "linux/err.h"
#include "linux/export.h"
#include "linux/gpio.h"
#include "linux/jump_label.h"
#include "linux/kdev_t.h"
#include "linux/mod_devicetable.h"
#include "linux/node.h"
#include "linux/of.h"
#include "linux/printk.h"
#include "linux/stddef.h"
#include "linux/types.h"
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/timer.h>
#include "linux/delay.h"


u8 DHT11_Init(void);//³õÊ¼»¯DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//¶ÁÈ¡Êý¾Ý
u8 DHT11_Read_Byte(void);//¶ÁÈ¡Ò»¸ö×Ö½Ú
u8 DHT11_Read_Bit(void);//¶ÁÈ¡Ò»Î»
u8 DHT11_Check(void);//¼ì²âDHT11
void DHT11_Rst(void);//¸´Î»DHT11   

#endif
