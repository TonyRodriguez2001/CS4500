#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h> 

//Define a module parameter named target_pid. This allows us to specify a PID value 
//when loading the module
static int target_pid = 0;
module_param(target_pid, int, 0644);  // Permissions for the module parameter
MODULE_PARM_DESC(target_pid, "Target PID to lookup");  

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

static int __init print_pid_task_init(void) {
    struct pid *pid_struct;          // Pointer to a kernel's internal PID structure
    struct task_struct *task;        // Pointer to the task structure of a process
		
    //log the chosen target PID 
    printk(KERN_INFO "Target PID: %d\n", target_pid);

    //convert the PID to its corresponding pid_struct
    pid_struct = find_get_pid(target_pid);
    if (!pid_struct) {
        printk(KERN_ERR "Failed to get pid_struct for PID %d\n", target_pid);
        return -ESRCH;
    }

    //get the task_struct associated with the pid_struct
    task = pid_task(pid_struct, PIDTYPE_PID);
    if (!task) {
        printk(KERN_ERR "Failed to get task_struct for PID %d\n", target_pid);
        return -ESRCH;
    }

    // Print details of the specified process to the kernel log
    printk(KERN_INFO "Process Name: %s\n", task->comm);
    printk(KERN_INFO "Process ID: %d\n", task->pid);
    printk(KERN_INFO "Process State: %ld (%s)\n", task->state, get_task_state_description(task->state));

    //traverse up the process tree until we reach the init process (PID 1)
    //log details of each parent process along the way.
    while (task->pid != 1) {
        task = task->parent; // Move to the parent process
        printk(KERN_INFO "Parent Process Name: %s\n", task->comm);
        printk(KERN_INFO "Parent Process ID: %d\n", task->pid);
        printk(KERN_INFO "Parent Process State: %ld (%s)\n", task->state, get_task_state_description(task->state));
    }

    return 0;
}

static void __exit print_pid_task_cleanup(void) {
    printk(KERN_INFO "Cleaning up\n");
}

//specifying to run print_pid_task_init when the module is initialized
module_init(print_pid_task_init);
//specifying to run print_pid_task_cleanup when the module is removed 
module_exit(print_pid_task_cleanup);

MODULE_LICENSE("GPL");