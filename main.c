/**
 * @file main.c
 * @brief Functions and data related to the Temperature sensor driver implementation
 *
 * Based on the implementation of the "scull" device driver, found in
 * Linux Device Drivers example code.
 *
 * @original author Dan Walkes
 * @date 2019-10-22
 * @copyright Copyright (c) 2019
 *
 * @adopted by Hyoun Cho for course work
 * @date 2023-08-24
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h> // file_operations
#include <linux/slab.h> // file_operations
#include <linux/random.h>

#include "tempdriver.h"
#include "temp_ioctl.h"

int temp_major =   0; // use dynamic major
int temp_minor =   0;

MODULE_AUTHOR("Hyoun Cho"); 
MODULE_LICENSE("Dual BSD/GPL");

struct temp_dev temp_device;

int read_count = 0;


int temp_open(struct inode *inode, struct file *filp)
{
    struct temp_dev *dev;

    PDEBUG("OPEN");

    dev = container_of(inode->i_cdev, struct temp_dev, cdev);

    filp->private_data = dev;

    return 0;
}

int temp_release(struct inode *inode, struct file *filp)
{
    PDEBUG("RELEASE");

    return 0;
}

ssize_t temp_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
    //struct temp_dev *dev;
    char write_buf[12];
    int temp, not_copied, n_read;

    PDEBUG("READ >>  %zu bytes with offset %lld, f_pos=%lld", count, *f_pos, filp->f_pos);

    if (filp == NULL) return -EFAULT;

    read_count++;

    if (read_count % 2 == 0) return 0;

    //dev = (struct temp_dev*) filp->private_data; 
    //if (dev == NULL) return -EFAULT;

    get_random_bytes(&temp, sizeof(temp)); // temp: 0~255

    temp = 76 + temp % 10; // jitter around 76 
					   
    // temp to jitter
    snprintf(write_buf, 12, "%d", temp); // 12 = buffer size

    n_read = 3;
    not_copied = copy_to_user(buf, write_buf, n_read); // two digits + '\0'
						       //
    *f_pos += n_read;
    
    return n_read;
}

ssize_t temp_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
    // struct temp_dev *dev;

    if (filp == NULL) return -EFAULT;
    
    // dev = (struct temp_dev*) filp->private_data; 
						 
    printk(KERN_ERR "write not supported");

    return 0;
}

loff_t temp_seek(struct file *filp, loff_t off, int whence)
{
    //struct temp_dev *dev;

    if (filp == NULL) return -EFAULT;

    //dev = (struct temp_dev*) filp->private_data; 

    printk(KERN_ERR "seek not supported");

    return -EINVAL;
}

long temp_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //struct temp_dev *dev;

    if (filp == NULL) return -EFAULT;

    //dev = (struct temp_dev*) filp->private_data;

    printk(KERN_ERR "ioctl not supported");

    return 0;
}

struct file_operations temp_fops = {
    .owner  = THIS_MODULE,
    .read   = temp_read,
    .write  = temp_write,
    .open   = temp_open,
    .llseek = temp_seek,
    .unlocked_ioctl  = temp_ioctl,
    .release= temp_release,
};

static int temp_setup_cdev(struct temp_dev *dev)
{
    int err, devno = MKDEV(temp_major, temp_minor);

    cdev_init(&dev->cdev, &temp_fops);

    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &temp_fops;

    err = cdev_add (&dev->cdev, devno, 1);
    if (err) {
        printk(KERN_ERR "Error %d adding temp cdev", err);
    }
    return err;
}

int temp_init_module(void)
{
    dev_t dev = 0;
    int result;

    printk(KERN_ALERT "Hello, this temp driver");

    // dynamic allocation of device number
    result = alloc_chrdev_region(&dev, temp_minor, 1, "tempdriver");
    temp_major = MAJOR(dev);

    if (result < 0) {
        printk(KERN_WARNING "Can't get major %d\n", temp_major);
        return result;
    }

    // initialize data
    memset(&temp_device,0,sizeof(struct temp_dev));

    // intialize data
    mutex_init(&temp_device.lock); // mutex init -- not used

    // device setup
    result = temp_setup_cdev(&temp_device);

    if( result ) {
        unregister_chrdev_region(dev, 1);
    }
    return result;
}

void temp_cleanup_module(void)
{
    dev_t devno = MKDEV(temp_major, temp_minor);

    cdev_del(&temp_device.cdev);

    // unregister device
    unregister_chrdev_region(devno, 1);
}

module_init(temp_init_module);
module_exit(temp_cleanup_module);
