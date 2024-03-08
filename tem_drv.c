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
#include "dht11.h"
int tem1; //gpio 设备号
static  const struct  of_device_id  tem_get_device[] = 
{
    {   
        .compatible = "tem_getdata",
        .name = ""
    }
};

int tem_open(struct inode * node , struct file * fe)
{
    printk("\n open form driver \n");
    return  0;
}
ssize_t tem_read(struct file *fp, char __user * buf, size_t n , loff_t * loff)
{
    char  read_buf[2];
    DHT11_Read_Data(&read_buf[0],&read_buf[1]);
    copy_to_user(buf,read_buf,n);
    return 2;
}
static struct file_operations tem_fp=
{
    .owner = THIS_MODULE,
    .open = tem_open,
    .read = tem_read 
};

#define MYDEV_CNT     1
int tem_getdata_probe(struct platform_device * tem_get_device ){
    struct device_node *tem_device_node;
    tem_device_node = of_find_node_by_path("/tem_getdata");
    if(tem_device_node == NULL)
    {
        printk(KERN_EMERG "\t  get tem_device_node failed!  \n");
    }
    int tem1 = of_get_named_gpio(tem_device_node,"tem1",0);
    gpio_direction_input(tem1);

    // 注册字符驱动
    int  ret ;
    unsigned int mydev_major = 0;
    dev_t mydev_num = 0;
    struct cdev * tem_cdev;
    struct class * tem_class;

    ret = alloc_chrdev_region(&mydev_num,0,1,"tem_dataget"); //分配设备号
    mydev_major = MAJOR(mydev_num); 
    if(ret < 0)
    {
        printk(KERN_INFO "alloc_chrdev_region fail\n");
        goto out_err_0;
    }
    printk(KERN_INFO "MAJOR %d\n", mydev_major); //打印主设备号
    // 为一个字符串 分配空间
    tem_cdev = cdev_alloc();

    cdev_init(tem_cdev,&tem_fp);  //  将字符设备与 file_operation 绑定
    cdev_add(tem_cdev,mydev_num,MYDEV_CNT); // 将字符设备与设备号绑定 并向内核注册
    if(ret)
    {
         printk(KERN_INFO "cdev_add error\n");
        goto out_err_1;
    }
    tem_class  = class_create(THIS_MODULE,"tem_class");
    if(IS_ERR(tem_class))
    {
        printk(KERN_ERR "can't register device mydevice class\n");
        goto out_err_2;
    }
    device_create(tem_class,NULL,mydev_num,NULL,"tem_device");

    return 0;

    out_err_2:
        cdev_del(tem_cdev);
    out_err_1:
        unregister_chrdev_region(mydev_num, MYDEV_CNT);
    out_err_0:
        return -EINVAL;

}
int tem_getdata_remove(struct platform_device *);

static  struct platform_driver tem_get_driver= 
{
    .probe  = tem_getdata_probe,
    .driver = {
        .name = "tem_getdata_platform",
        .owner  = THIS_MODULE,
        .of_match_table  =  tem_get_device
    },
    .remove = tem_getdata_remove

};

static int __init tem_getdata_driver_init(void)
{
    int error;

    error = platform_driver_register(&tem_get_driver);

    printk(KERN_EMERG "\tDriverState = %d\n",error);
    return 0;
}
static void __exit tem_getdata_driver_exit(void)
{
    printk(KERN_EMERG "platform_driver_exit!\n");

    platform_driver_unregister(&tem_get_driver);
}

module_init(tem_getdata_driver_init);
module_exit(tem_getdata_driver_exit);
MODULE_LICENSE("GPL");


