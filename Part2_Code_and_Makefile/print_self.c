#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

//gets description for task state
const char* get_task_state_description(long state) {
    switch(state) {
        case TASK_RUNNING:
            return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE:
            return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE:
            return "TASK_UNINTERRUPTIBLE";
        case TASK_STOPPED:
            return "TASK_STOPPED";
        case TASK_TRACED:
            return "TASK_TRACED";
        case EXIT_ZOMBIE:
            return "EXIT_ZOMBIE";
        case EXIT_DEAD:
            return "EXIT_DEAD";
        default:
            return "UNKNOWN_STATE";
    }
}

static int __init print_self_init(void) {
    //"current" is a macro that points to the task_struct of the currently executing process
    struct task_struct *task = current; 

    //log that the module initialization has started
    printk(KERN_INFO "Print Self Module: Initialization started\n");

    // Print details of the current process to the kernel log
    printk(KERN_INFO "Process Name: %s\n", task->comm);
    printk(KERN_INFO "Process ID: %d\n", task->pid);
    printk(KERN_INFO "Process State: %ld (%s)\n", task->state, get_task_state_description(task->state));

    //Traverse up the process tree until we reach the init process (PID 1)
    //Log details of each parent process
    while (task->pid != 1) {
        task = task->parent; // Move to the parent process
        printk(KERN_INFO "Parent Process Name: %s\n", task->comm);
        printk(KERN_INFO "Parent Process ID: %d\n", task->pid);
        printk(KERN_INFO "Parent Process State: %ld (%s)\n", task->state, get_task_state_description(task->state));
    }

    return 0;
}

// This function is called when the module is removed from the kernel.
static void __exit print_self_cleanup(void) {
    // Log that the module cleanup process has started
    printk(KERN_INFO "Print Self Module: Cleaning up\n");
}

//specifying to run the print_self_init function when the module is initialized
module_init(print_self_init);

//specifying to run the print_self_cleanup function when the module is removed
module_exit(print_self_cleanup);
MODULE_LICENSE("GPL");