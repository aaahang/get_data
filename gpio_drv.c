#include "asm/io.h"
#include "asm/uaccess.h"
#include "linux/dmi.h"
#include "linux/export.h"
#include "linux/gpio.h"
#include "linux/jump_label.h"
#include "linux/kdev_t.h"
#include "linux/mod_devicetable.h"
#include "linux/node.h"
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
/*确定主设备号，也可以让内核分配；
1.定义自己的 file_operations 结构体，这是核心；
2.实现对应的 drv_open/drv_read/drv_write 等函数，填入 file_operations 结构
体；
3.把 file_operations 结构体告诉内核：通过 register_chrdev 函数；
谁来注册驱动程序？需要一个入口函数：安装驱动程序时，就会去调用这个
入口函数；
4.有入口函数就应该有出口函数：卸载驱动程序是，出口函数调用
unregister_chrdev;
5.其它完善：提供设备信息，自动创建设备节点：
class_create,device_create;*/
static int major ;
static struct class *gpio_class;
static char kernel_buf[1024];
//Address: 229_0000h base + 14h offset = 229_0014h
volatile static unsigned int *SW_MUX_GPIO5_3;
//20A_C000
volatile static unsigned int *GPIO5_DR;
//20A_C004
volatile static unsigned int *GPIO5_GDIR;
#define a "asdasdasd"
static int  gpio_open(struct inode *node, struct file *file)
{
    // 引脚初始化，
    *SW_MUX_GPIO5_3 &= ~0Xf;
    *SW_MUX_GPIO5_3 |= 0x5;
    *GPIO5_GDIR |= 0x8;
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

ssize_t gpio_read(struct file *file, char __user *buf, size_t n, loff_t * sefft)
{
    printk("%s %s %d\n",__FILE__,__FUNCTION__,__LINE__);
    copy_to_user(buf, kernel_buf,n);
    return n;
}
static ssize_t gpio_write(struct file *file, const char __user *buf, size_t n, loff_t * sefft)
{ 
    char val;
    copy_from_user(&val,buf,1);
    printk("%s %s %d %s\n",__FILE__,__FUNCTION__,__LINE__);
    if(val)
    {
    *GPIO5_DR = 0x0;
    }
    else
    {
        *GPIO5_DR = 1<<3;
    }
    return n;
}
static int gpio_release(struct inode *inode, struct file *file)
{
    printk("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
    return 0;      
}
static struct file_operations gpio_drv = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .release = gpio_release,
    .read = gpio_read,
    .write = gpio_write,
    
};
//创建一个驱动led的设备 insmod时（安装驱动程序的时候）调用这个函数
static int __init gpio_drv_init(void)
{   
    /*注册一个设备，主设备号由系统自动分配（写0即自动分配设备号），并将gpio_drv传入设备 并将设备命名为“led” 
    返回的major是主设备号*/
    major = register_chrdev(0,"gpio_led",&gpio_drv);

    printk("%s %s %d \n",__FILE__,__FUNCTION__,__LINE__);
    /*创建一个类，在这个模型中*/
    gpio_class = class_create(THIS_MODULE, "gpio_led");
    	if (IS_ERR(gpio_class)) {
		printk("failed to allocate class\n");
		return PTR_ERR(gpio_class);
	}
    /*1.创建设备节点
    2.空（不知道这写什么）
    3.设置设备号
    4 设置设备名称
    */
    device_create(gpio_class,NULL,MKDEV(major, 0),NULL,"gpio_led");


    //Address: 229_0000h base + 14h offset = 229_0014h
    SW_MUX_GPIO5_3 = ioremap(0x2290014, 4);
    //20A_C000
    GPIO5_DR = ioremap(0x20AC000,4);
    //20A_C004
    GPIO5_GDIR = ioremap(0x20AC004,4);
    return 0;
}
//删除驱动led的设备 unsomd（卸载驱动程序的时候）调用这个函数
static  void   __exit gpio_drv_exit(void)
{ 
    iounmap(SW_MUX_GPIO5_3);
    //20A_C000
    iounmap(GPIO5_DR);
    
    //20A_C004
    iounmap(GPIO5_GDIR);
    //注销这个设备
    device_destroy(gpio_class,"gpio_led");
    //注销这个类
    class_destroy(gpio_class);
    //注销这个驱动
    unregister_chrdev(major,"gpio_led");
    

}
module_init(gpio_drv_init);
module_exit(gpio_drv_exit);
MODULE_LICENSE("GPL"); 


