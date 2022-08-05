/**
 * @file   irqgen_cdev.c
 * @author Nicola Tuveri
 * @date   15 November 2018
 * @version 0.7
 * @target_device Xilinx PYNQ-Z1
 * @brief   A stub module to support the IRQ Generator IP block for the
 *          Real-Time System course (character device support).
 */

# include <linux/kernel.h>           // Contains types, macros, functions for the kernel

# include <linux/device.h>
# include <linux/string.h>

# include <linux/cdev.h>             // Header for character devices support
# include <linux/fs.h>               // Header for Linux file system support
# include <linux/uaccess.h>          // Header for userspace access support

# include "irqgen.h"                 // Shared module specific declarations

#define IRQGEN_CDEV_CLASS "irqgen-class"

struct irqgen_chardev {
    struct cdev cdev;
    dev_t devt;
    struct device *dev;
    struct class *class;

    // TODO: do we need a sync mechanism for any cdev operation?
};


static struct irqgen_chardev irqgen_chardev;

// The prototype functions for the character driver -- must come before the struct definition
static int     irqgen_cdev_open(struct inode *, struct file *);
static int     irqgen_cdev_release(struct inode *, struct file *);
static ssize_t irqgen_cdev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops = {
    .open = irqgen_cdev_open,
    .release = irqgen_cdev_release,
    .read = irqgen_cdev_read,
};

// Initialize the char device driver
int irqgen_cdev_setup(struct platform_device *pdev)
{
    int ret;

    cdev_init(&irqgen_chardev.cdev, &fops);
    irqgen_chardev.cdev.owner = THIS_MODULE;
    irqgen_chardev.cdev.kobj.parent = &pdev->dev.kobj;
    irqgen_chardev.class = class_create(THIS_MODULE, IRQGEN_CDEV_CLASS); 

	// TODO: dinamically allocate a major and a minor for this chrdev
    // don't forget error handling
    ret = alloc_chrdev_region(&irqgen_chardev.cdev.dev, 0, 1, "irqgen"); // first minor number to use is 0
    if(ret != 0){
        printk(KERN_ERR "Failed to dynamically allocate minor and major\n");
        goto err_alloc_chrdev_region;
    }
	// TODO: add to the system the cdev for the allocated (major,minor)
    // don't forget error handling
    ret = cdev_add(&irqgen_chardev.cdev, irqgen_chardev.cdev.dev, 1);
    if(ret < 0){
        printk(KERN_ERR "Failed to add to the system the cdev for the allocated (major,minor)\n");
        goto err_cdev_add;
    }
	// Add an "irqgen" node in the /dev/ filesystem (hint: device_create())
    // don't forget error handling
    irqgen_chardev.dev = device_create(irqgen_chardev.class, NULL, irqgen_chardev.cdev.dev, NULL, "irqgen");
    if(irqgen_chardev.dev == NULL){
        printk(KERN_ERR "Failed to add an irqgen node in the /dev/ filesystem\n");
        goto err_device_create;
    }

    // TODO: do we need a sync mechanism for any cdev operation?
	return 0;

	// TODO: use labels to handle errors and undo any resource allocation	
	err_alloc_chrdev_region:
		unregister_chrdev_region(irqgen_chardev.cdev.dev, 1);
		return ret;
	
	err_cdev_add:
		cdev_del(&irqgen_chardev.cdev);
		return ret;
		
	err_device_create:
		device_destroy(irqgen_chardev.class, irqgen_chardev.cdev.dev);
		class_destroy(irqgen_chardev.class);
		return ret;
}

void irqgen_cdev_cleanup(struct platform_device *pdev)
{
    // destroy, unregister and free, in the right order, all resources
    // allocated in irqgen_cdev_setup()
    printk(KERN_INFO "Cleanup cdev\n");

    device_destroy(irqgen_chardev.class, irqgen_chardev.cdev.dev);
    cdev_del(&irqgen_chardev.cdev);
    unregister_chrdev_region(irqgen_chardev.cdev.dev, 1);
	class_destroy(irqgen_chardev.class);
}

static u8 already_opened = 0;

static int irqgen_cdev_open(struct inode *inode, struct file *f)
{
# ifdef DEBUG
    printk(KERN_DEBUG KMSG_PFX "irqgen_cdev_open() called.\n");
# endif

    if (already_opened) {
        return -EBUSY;
    }
    already_opened = 1;
    return 0;
}

static int irqgen_cdev_release(struct inode *inode, struct file *f)
{
# ifdef DEBUG
    printk(KERN_DEBUG KMSG_PFX "irqgen_cdev_release() called.\n");
# endif

    if (!already_opened) {
        return -ECANCELED;
    }
    already_opened = 0;
    return 0;
}

// This will write in the userspace buffer one line at a time, the
// client (or the std library) will retry automatically.
//
// NOTE: We require the userland buffer to be at least 60 bytes (count>=60)
static ssize_t irqgen_cdev_read(struct file *fp, char *ubuf, size_t count, loff_t *f_pos)
{
    // We cannot write to user space directly: we need to use a buffer
    // in kernel space and then use specialized functions to copy the
    // data to provided the userland buffer.
#define KBUF_SIZE 100
    static char kbuf[KBUF_SIZE];
    ssize_t ret = 0;
    unsigned long ret_copy_to_user;

    struct latency_data v;

    if (count < 60) {
        printk(KERN_ERR KMSG_PFX "read() buffer too small (<=60).\n");
        return -ENOBUFS;
    }

	// TODO: how to protect access to shared r/w members of irqgen_data?
    spin_lock_irqsave(&irqgen_data->irqgen_spinlock, irqgen_data->irqgen_flags);
	
    if (irqgen_data->rp == irqgen_data->wp) {
        // Nothing to read
        spin_unlock_irqrestore(&irqgen_data->irqgen_spinlock, irqgen_data->irqgen_flags);
        return 0;
    }

    v = irqgen_data->latencies[irqgen_data->rp];
    irqgen_data->rp = (irqgen_data->rp + 1)%MAX_LATENCIES;

    ret = scnprintf(kbuf, KBUF_SIZE, "%u,%lu,%llu\n", v.line, v.latency, v.timestamp);
    if (ret < 0) {
        goto end;
    } else if (ret == 0) {
        ret = -ENOMEM;
        goto end;
    }
	// TODO: how to transfer from kernel space to user space?
    ret_copy_to_user  = copy_to_user(ubuf, kbuf, ret);

	if(ret_copy_to_user == 0){
        *f_pos += ret;
        goto end;
	}
    else{
        ret = -EIO;
        goto end; 
    }	

 end:
    spin_unlock_irqrestore(&irqgen_data->irqgen_spinlock, irqgen_data->irqgen_flags);
    return ret;

#undef KBUF_SIZE
}


