#include <linux/kernel.h>           // Low-level kernel macros
#include <linux/module.h>           // Needed for dynamic module loading
#include <linux/interrupt.h>        // Tasklets
#include <linux/slab.h>             // kmalloc
#include <linux/device.h>           // sysfs functions

#define SYSFS_FILE_ATTR_NAME "evil"

#define STORAGE_SIZE PAGE_SIZE // DON'T CHANGE IN FINAL REVISION
#define INPUT_BUFSIZE 1000     // DON'T CHANGE IN FINAL REVISION

// Dynamic and static allocation for the sake of example
char *data_storage = NULL;
char input_buf[INPUT_BUFSIZE];

//struct tasklet_struct* tasklet = NULL;
//remove null initialization of tasklet
struct tasklet_struct* tasklet;
int32_t bytes_stored = 0;
// A standalone kobject for a sysfs entry
static struct kobject* evil_kobj = NULL;

static void do_tasklet(unsigned long data)
{
    int32_t retval;

    if(bytes_stored+strlen((char *)data) >= STORAGE_SIZE-1) {
        printk(KERN_INFO "EVIL: storage full\n");
        return;
    }

    // Replace 'a's with ' ' in the name of evilness
    strreplace((char *)data, 'a', ' ');

    retval = sprintf(&data_storage[bytes_stored], "%s", (char *)data);
    if(retval < 0) {
        printk(KERN_ERR "EVIL: sprintf failed\n");
    } else {
        // Null-character excluded from the sprintf return value so 1 should be added
        bytes_stored += retval+1;
		
		//if byte in the buf over the limit
		if(bytes_stored >= STORAGE_SIZE){
			return -ENOMEM;
		}
		else{
			printk(KERN_INFO "EVIL: bytes stored: %d\n", bytes_stored);
		}
    }
}

// The sysfs attribute invoked when writing
static ssize_t store_evil(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    int32_t retval;
	
	//if buf size larger than input_buf's size
    if(strlen(buf)  > INPUT_BUFSIZE-1){ 
		printk(KERN_INFO "EVIL: File too large\n");
        return -EFBIG;
	}	    
	else{
		
		//write content of buf to input_buf
    	retval = sprintf(input_buf, "%s", buf);
		if(retval < 0) { //fail to write content
		    printk(KERN_ERR "EVIL: sprintf failed\n");
		
		}
		
		//if number of bytes already in buf and writing bytes greater than STORAGE_SIZE
        if(strlen(buf) + bytes_stored >= STORAGE_SIZE - 1){
            printk(KERN_ERR "EVIL: storage full\n");
		
        }
        // Run a tasklet to perform string manipulation and storing the data
		tasklet_schedule(tasklet);
        
	}
    
    return count;
}

// The sysfs attribute invoked when reading from the file
static ssize_t show_evil(struct device *dev, struct device_attribute *attr, char *buf) {
	
	if(bytes_stored <1){ //if file is empty
		return 0;
	}	
	
	//if anything available to read.
	else if(bytes_stored >1){
				
		//number of byte written to the buffer
		uint32_t bytes = 0;
        int32_t retval;
		
		// Go through the data storage and write all found strings to the output buffer
        //if byte_stored>bytes, increase byte til it reachs the end of the file.
		while(bytes < bytes_stored){
			retval = sprintf(&buf[bytes], "%s", &data_storage[bytes]);
			
			/*if(retval =< 0) { 
            
                break;
            }*/
			
			//if sprintf failed
			if(retval < 0){ 
				printk(KERN_ERR "EVIL: sprintf failed\n");
			}
		    
			//if number of bytes read more than STORAGE_SIZE
            else if(bytes > STORAGE_SIZE - 1){ 
                printk(KERN_INFO "EVIL: File too large");
                return -EFBIG;
            }
			
			// Null-character excluded from the sprintf return value so 1 should be added
			bytes += retval + 1;
		}
		
		printk("MUAHAHAHA\n");
		
		return bytes;
	}
}

//
//-Kobject attributes declaration
//
static struct device_attribute dev_attr_evil = {
    .attr = {
        .name = SYSFS_FILE_ATTR_NAME,
        .mode = S_IRUGO|S_IWUSR,  //add write permission
    },
    .show = show_evil,
    .store = store_evil,
};


// The kernel module init function
static int32_t __init evil_init(void)
{
    int32_t retval;

    printk(KERN_INFO "EVIL: THIS IS THE EVIL MODULE\n");

    data_storage = (char*)kmalloc(STORAGE_SIZE, GFP_KERNEL);
    if(data_storage == NULL) {
        printk(KERN_ERR "EVIL: data storage memory allocation failed\n");
        retval = -ENOMEM;
        goto error_alloc_data_storage;
    }

    // Create a sysfs directory entry under /sys/kernel/
    evil_kobj = kobject_create_and_add("evil_module", kernel_kobj);
    if(evil_kobj == NULL) {
        printk(KERN_ERR "EVIL: kobject_create_and_add failed\n");
        retval = -EINVAL;
        goto error_kobject_create;
    }

    // Add attributes to the kobject
    // The attributes are presented as a file in the created directory
    retval = sysfs_create_file(evil_kobj, &dev_attr_evil.attr);
    if(retval) {
        printk(KERN_ERR "EVIL: sysfs_create_file failed\n");
        goto error_sysfs_create;
    }

    // Initialize the tasklet
	//allocate memory for tasklet using kmalloc
    tasklet= kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
    tasklet_init(tasklet, do_tasklet, (unsigned long)input_buf);
	
    return 0;

	 error_sysfs_create:
		kobject_del(evil_kobj);
	 error_kobject_create:
		kfree(data_storage);
	 error_alloc_data_storage:
		printk(KERN_ERR "EVIL: error occurred while evil init!\n");
    return retval;
}

// The kernel module exit function
static void __exit evil_exit(void)
{
    //switch kfree and tasklet_kill to prevent segmentation fault
    tasklet_kill(tasklet);
    kfree(tasklet);
    kobject_del(evil_kobj);
    sysfs_remove_file(evil_kobj, &dev_attr_evil.attr);
    kfree(data_storage);

    printk(KERN_INFO "EVIL: MUAHAHAHA\n");
}

module_init(evil_init);
module_exit(evil_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("The evil kernel module for the Real-time systems course");
MODULE_AUTHOR("Jan Lipponen <jan.lipponen@wapice.com>");
MODULE_AUTHOR("Le Do");
MODULE_AUTHOR("Robert");
MODULE_VERSION("1.0");
