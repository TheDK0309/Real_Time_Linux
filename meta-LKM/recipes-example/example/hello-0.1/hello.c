#include <linux/init.h>             /* Macros used to mark up functions e.g., __init*/ __exit
#include <linux/module.h>           /* Core header for loading LKMs into the kernel*/
#include <linux/kernel.h>           /* Contains types, macros, functions for the kernel*/

MODULE_LICENSE("GPL");              /*lincense*/
MODULE_AUTHOR("Le Do");      /* The author(s)*/
MODULE_AUTHOR("Robert Cantaragiu");
MODULE_DESCRIPTION("Linux Driver for Exercise 2");  /* The description */
MODULE_VERSION("0.1");              /* The version of the module*/

static char *name = "Group 7";        
module_param(name, charp, S_IRUGO); /* Param desc. charp = char ptr, S_IRUGO can be read/not changed */
MODULE_PARM_DESC(name, "The name to display in /var/log/messages");  /* parameter description */

/*LKM initialization function*/
static int __init helloBBB_init(void){
   printk(KERN_INFO "Hello %s let's have fun together!!\n", name);
   return 0;
}


/*LKM clean up function*/
static void __exit helloBBB_exit(void){
   printk(KERN_INFO "Goodbye %s, i will miss you!!\n", name);
}

/*
identify the initialization function at insertion time and the cleanup function
*/
module_init(helloBBB_init);
module_exit(helloBBB_exit);
