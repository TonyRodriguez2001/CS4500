#include <linux/module.h>
#include <linux/kernel.h>

//called when the module is loaded into the kernel
static int hello_init(void) {
    //Printing the message to the kernel log 
    printk(KERN_INFO "Hello world!\n"); 
    return 0; 
}

//called when the module is removed from the kernel
static void hello_exit(void) {
    //Printing the message to the kernel log
    printk(KERN_INFO "Goodbye world!\n");
}

//specifying to run the hello_init function when the module is inserted
module_init(hello_init);

//specifying to run the hello_exit function when the module is removed
module_exit(hello_exit);

MODULE_LICENSE("GPL");