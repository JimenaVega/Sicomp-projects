#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Jimena Vega Cuevas");


//headers
int init_module(void);
void exit_module(void);

static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);


#define DEVICE_NAME "encryptor"
#define BUF_LEN 80

static int major;
static int dev_open = 0;
static char msg[BUF_LEN];
static char *message; //msg_Ptr


static struct file_operations fops = {
    .read    = device_read,
	.write   = device_write,
	.open    = device_open,
	.release = device_release
};

int init_encryptor(void){

    major = register_chrdev(0, DEVICE_NAME, &fops);

    if(major < 0){
        printk(KERN_ALERT "%s failed with %d\n","Sorry, registering the character device ", major);
		return major;
    }

    printk(KERN_INFO "Encryptor module registered with: %d\n", major);
    printk(KERN_INFO "'mknod /dev/%s c %d 0.\n", DEVICE_NAME, major);

    return 0;
}

void exit_encryptor(void){
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Encryptor: module unregistered.");
}

static int device_open(struct inode *inode, struct file *file){

	//static int counter = 1;
	if(dev_open){
		return -EBUSY;
	}

	dev_open++;
	//sprintf(msg, "Encryptor open %d times", counter++);
	message = msg;
	try_module_get(THIS_MODULE);

	return 0;
}

static int device_release(struct inode *inode, struct file *file){
	dev_open--;
	module_put(THIS_MODULE);

	printk(KERN_INFO "Encryptor: closed device\n");
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset){

	copy_from_user(msg, buffer, len);

	printk(KERN_INFO "Encryptor input:  %s", msg);
	
	int i;
	for(i = 0; i < len; i++) {
		msg[i] = msg[i]+2;
	}
	printk(KERN_INFO "Encryptor output: %s", msg);
	
	return len;
}


static ssize_t device_read(struct file *filp, char *buffer,	size_t length, loff_t * offset){

	int bytes_read = 0;

	if (*message == 0)
		return 0;

	while (length && *message){

		put_user(*(message++), buffer++);
		length--;
		bytes_read++;
	}
	printk(KERN_INFO "Encryptor output READ: %s\n", msg);

	//printk(KERN_INFO "Encryptor:  %d characters sended to user %s\n", bytes_read, buffer);
	return bytes_read;
}

module_init(init_encryptor);
module_exit(exit_encryptor);







