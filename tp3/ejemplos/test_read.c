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
#include <linux/gpio.h>

//Timer Variable
#define TIMEOUT 6000    //milliseconds
#define LEN 128

/* Define GPIOs for BUTTONS */
static struct gpio buttons[] = {
		{ 8, GPIOF_IN, "BUTTON 1" },
		{ 12, GPIOF_IN, "BUTTON 2" },
};

//Timer
static struct timer_list etx_timer;
static unsigned int count = 0;

static dev_t first; 		// Global variable for the first device number
static struct cdev c_dev; 	// Global variable for the character device structure
static struct class *cl; 	// Global variable for the device class

static char msg[LEN];              // buffer en espacio de kernel

static char c;
int counter = 0;
//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
    /* do your timer stuff here */
    printk(KERN_INFO "JIMEtest_read: Timer Interrupt [%d]", count++);
    printk(KERN_INFO "JIMEtest_read: Current button0 value: %d\n", gpio_get_value(buttons[0].gpio));
    printk(KERN_INFO "JIMEtest_read: Current button1 value: %d\n", gpio_get_value(buttons[1].gpio));
    
    //BORRAR
    if(counter == 1){
        counter = 0;
         c = 'N';
    }else{
        counter = 1;
        c = 'S';
    }
   
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

static int test_read_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "test_read: open()\n");
    return 0;
}
static int test_read_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "test_read: close()\n");
    return 0;
}

static ssize_t test_read_read(struct file *f, char __user *buf, size_t len, loff_t *offset)
{
    printk(KERN_INFO "test_read: read()\n");
    
    //printk("Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if(*offset==0)
    {
        if (copy_to_user(buf, &c, 1)) {
            return -EFAULT;
        } else
        {
            printk("Data read by user: %c\n", c);
            (*offset) ++;
            return 1;
        }

    } else 
    {
        return 0;
    }

}

// my_write escribe "len" bytes en "buf" y devuelve la cantidad de bytes escrita, 
// que debe ser igual "len".

// Cuando hago un $ echo "bla bla bla..." > /dev/SdeC_drv3, se convoca a my_write.!!

static ssize_t test_read_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "test_read: write()\n");

    if(copy_from_user(msg, buf, len) != 0)
   // if(copy_from_user(&c, buf, len) != 0)
        return -EFAULT;
    else
        printk(KERN_INFO "test_read: writed: %s", msg);
    return len;
}

static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = test_read_open,
    .release = test_read_close,
    .read = test_read_read,
    .write = test_read_write
};

static int __init test_read_init(void) /* Constructor */
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "JIME  NEW test_read: Module Init.\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "test_read")) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "test_read")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "test_read")))
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

    // register BUTTON gpios
	ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs for BUTTONs: %d\n", ret);
		return ret;
	}

    return 0;
}

static void __exit test_read_exit(void){ /* Destructor */
    gpio_free_array(buttons, ARRAY_SIZE(buttons));
    del_timer(&etx_timer);
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "JIME test_read: Module Exit.\n");
}

module_init(test_read_init);
module_exit(test_read_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alejo-Jime");
MODULE_DESCRIPTION("GPIO - Raspberry Pi 3 B+.");