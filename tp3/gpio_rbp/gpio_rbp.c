#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/timer.h>

//Timer Variable
#define TIMEOUT 5000    //milliseconds
#define LEN 128

//Timer
static struct timer_list etx_timer;
static unsigned int count = 0;

static dev_t first; 		// Global variable for the first device number
static struct cdev c_dev; 	// Global variable for the character device structure
static struct class *cl; 	// Global variable for the device class

static char msg[LEN];              // buffer en espacio de kernel
static char c;

//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
    /* do your timer stuff here */
    printk(KERN_INFO "gpio_rbp: Timer Interrupt [%d]", count++);
    // pr_info("Timer Callback function Called [%d]\n",count++);
    
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int gpio_rbp_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "gpio_rbp: open()\n");
    return 0;
}
static int gpio_rbp_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "gpio_rbp: close()\n");
    return 0;
}

static ssize_t gpio_rbp_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "gpio_rbp: read()\n");

    if (*off == 0)
    {
        if (copy_to_user(buf, msg, LEN) != 0)
            return -EFAULT;
        else
        {
            (*off)++;
            return 1;
        }
    }
    else
        return 0;

}

// my_write escribe "len" bytes en "buf" y devuelve la cantidad de bytes escrita, 
// que debe ser igual "len".

// Cuando hago un $ echo "bla bla bla..." > /dev/SdeC_drv3, se convoca a my_write.!!

static ssize_t gpio_rbp_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "gpio_rbp: write()\n");

    // if ( copy_from_user(&c, buf + len - 1, 1) != 0 )
    //     return -EFAULT;
    // else
    //     return len;

    if(copy_from_user(msg, buf, len) != 0)
        return -EFAULT;
    else
        printk(KERN_INFO "gpio_rbp: writed: %s", msg);
        return len;
}

static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = gpio_rbp_open,
    .release = gpio_rbp_close,
    .read = gpio_rbp_read,
    .write = gpio_rbp_write
};

static int __init gpio_rbp_init(void) /* Constructor */
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "gpio_rbp: Module Init.\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "gpio_rbp")) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "gpio_rbp")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "gpio_rbp")))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &pugs_fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

    /* setup your timer to call my_timer_callback */
    timer_setup(&etx_timer, timer_callback, 0);

    /* setup timer interval to based on TIMEOUT Macro */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));

    return 0;
}

static void __exit gpio_rbp_exit(void){ /* Destructor */
    del_timer(&etx_timer);
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "gpio_rbp: Module Exit.\n");
}

module_init(gpio_rbp_init);
module_exit(gpio_rbp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alejo-Jime");
MODULE_DESCRIPTION("GPIO - Raspberry Pi 3 B+.");
