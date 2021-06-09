/**
 * @file mychardev.c
 * @author https://olegkutkov.me/2018/03/14/simple-linux-character-device-driver/
 * @brief 
 * @version 0.1
 * @date 2021-06-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/timekeeping.h>
#include <linux/math64.h>


#define MAX_DEV 1

static int mychardev_open(struct inode *inode, struct file *file);
static int mychardev_release(struct inode *inode, struct file *file);
static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
static char c;              // buffer en espacio de kernel

static const struct file_operations mychardev_fops = {
    .owner      = THIS_MODULE,
    .open       = mychardev_open,
    .release    = mychardev_release,
    .unlocked_ioctl = mychardev_ioctl,
    .read       = mychardev_read,
    .write       = mychardev_write
};

struct mychar_device_data {
    struct cdev cdev;
};

static int dev_major = 0;
static int counter=0;
static time64_t tml1;
static time64_t tml2;
static time64_t tmd1=0;
static time64_t tmd2=0;

static struct class *mychardev_class = NULL;
static struct mychar_device_data mychardev_data[MAX_DEV];

static int mychardev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}


/* Define GPIOs for LEDs */
static struct gpio leds[] = {
		{  4, GPIOF_OUT_INIT_LOW, "LED 1" },
};

/* Define GPIOs for BUTTONS */
static struct gpio buttons[] = {
		{ 17, GPIOF_IN, "BUTTON 1" },	// turns LED on
		{ 18, GPIOF_IN, "BUTTON 2" },	// turns LED off
};

/* Later on, the assigned IRQ numbers for the buttons are stored here */
static int button_irqs[] = { -1, -1 };

/*
 * The interrupt service routine called on button presses
 */
static irqreturn_t button_isr(int irq, void *data)
{
    
    if(irq == button_irqs[0])
    {

	
	
	if(counter==0)
	{
	    tml1= ktime_get_seconds();
	}
	if(counter<10)
	{
	    counter++;
	    return IRQ_HANDLED;
	}
	tml2= ktime_get_seconds();
	
	printk(KERN_INFO "Current button1 value: %lld  char %i \n", tml2-tml1, (uint8_t)(tml2-tml1));
	char a=(tml2-tml1);
	c = a;
	counter=0;
	gpio_set_value(leds[0].gpio, !gpio_get_value(leds[0].gpio));
	return IRQ_HANDLED;
    }else if(irq == button_irqs[1])
    {
	if(tmd1==0){
	    gpio_set_value(leds[0].gpio, 1);
	    tmd1= ktime_get_ns();
	    return IRQ_HANDLED;
	}
	
	tmd2= ktime_get_ns();	
	
	ktime_t value=(tmd2-tmd1);
	value=div_u64(value,58000);
	c =(uint8_t) value;
	printk(KERN_INFO "Current button1 value: %lld  char %i \n", value, (uint8_t)value);
	gpio_set_value(leds[0].gpio, 0);
	tmd1=0;
    }

    return IRQ_HANDLED;
}

    	

static int __init mychardev_init(void)
{
    int err, i, ret;
    dev_t dev;

    err = alloc_chrdev_region(&dev, 0, MAX_DEV, "SisComChardev");

    dev_major = MAJOR(dev);

    mychardev_class = class_create(THIS_MODULE, "SisComChardev");
    mychardev_class->dev_uevent = mychardev_uevent;

    for (i = 0; i < MAX_DEV; i++) {
        cdev_init(&mychardev_data[i].cdev, &mychardev_fops);
        mychardev_data[i].cdev.owner = THIS_MODULE;

        cdev_add(&mychardev_data[i].cdev, MKDEV(dev_major, i), 1);

        device_create(mychardev_class, NULL, MKDEV(dev_major, i), NULL, "SisComChardev-%d", i);
    }

    /* Registro de botones */


	ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));

	if (ret) {
		printk(KERN_ERR "Unable to request GPIOs for BUTTONs: %d\n", ret);
		goto fail1;
	}

	printk(KERN_INFO "Current button1 value: %d\n", gpio_get_value(buttons[0].gpio));

	ret = gpio_to_irq(buttons[0].gpio);

	if(ret < 0) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		goto fail2;
	}

	button_irqs[0] = ret;

	printk(KERN_INFO "Successfully requested BUTTON1 IRQ # %d\n", button_irqs[0]);

	ret = request_irq(button_irqs[0], button_isr, IRQF_TRIGGER_RISING  /* | IRQF_DISABLED */, "gpiomod#button1", NULL);

	if(ret) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		goto fail2;
	}

ret = gpio_to_irq(buttons[1].gpio);

	if(ret < 0) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		goto fail2;
	}

	button_irqs[1] = ret;

	printk(KERN_INFO "Successfully requested BUTTON2 IRQ # %d\n", button_irqs[1]);

	ret = request_irq(button_irqs[1], button_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING /* | IRQF_DISABLED */, "gpiomod#button2", NULL);
	
	if(ret) {
		printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
		goto fail3;
	}

	return 0;

// cleanup what has been setup so far
fail3:
	free_irq(button_irqs[0], NULL);

fail2:
	gpio_free_array(buttons, ARRAY_SIZE(leds));

fail1:
	gpio_free_array(leds, ARRAY_SIZE(leds));

	return ret;
}

static void __exit mychardev_exit(void)
{
    int i;

    for (i = 0; i < MAX_DEV; i++) {
        device_destroy(mychardev_class, MKDEV(dev_major, i));
    }

    class_unregister(mychardev_class);
    class_destroy(mychardev_class);

    unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK);

    // free irqs
	free_irq(button_irqs[0], NULL);
	free_irq(button_irqs[1], NULL);

	// turn all LEDs off
	for(i = 0; i < ARRAY_SIZE(leds); i++) {
		gpio_set_value(leds[i].gpio, 0);
	}

	// unregister
	gpio_free_array(leds, ARRAY_SIZE(leds));
	gpio_free_array(buttons, ARRAY_SIZE(buttons));
}

static int mychardev_open(struct inode *inode, struct file *file)
{
    printk("SisComChardev: Device open\n");
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
    printk("SisComChardev: Device close\n");
    return 0;
}

static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("SisComChardev: Device ioctl\n");
    return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    printk("Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));


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

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t maxdatalen = 30, ncopied;
    uint8_t databuf[maxdatalen];

    printk("Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));


    if ( copy_from_user(&c, buf, 1) != 0 )
        return -EFAULT;


    printk("Data from the user: %c\n", c);

    return 1;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oleg Kutkov <elenbert@gmail.com>");

module_init(mychardev_init);
module_exit(mychardev_exit);