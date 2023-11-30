#include <linux/module.h>       // Needed by all modules
#include <linux/kernel.h>       // Needed for KERN_INFO
#include <linux/sched.h>        // For task_struct
#include <linux/pid.h>          // For pid types
#include <linux/mm.h>           // For mm_struct

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A module to calculate virtual address space size of a 
process");

static int pid = -1;
module_param(pid, int, 0644);
MODULE_PARM_DESC(pid, "Process ID to calculate virtual address space 
size");

static int __init va_space_init(void) {
    struct task_struct *task;
    struct pid *pid_struct;
    struct mm_struct *mm;
    unsigned long vsize = 0;

    if (pid < 0) {
        printk(KERN_INFO "Invalid PID\n");
        return -EINVAL;
    }

    pid_struct = find_get_pid(pid);
    if (!pid_struct) {
        printk(KERN_INFO "No such PID found\n");
        return -ESRCH;
    }

    task = pid_task(pid_struct, PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "No such task found\n");
        return -ESRCH;
    }

    mm = task->mm;
    if (mm) {
        struct vm_area_struct *vma = mm->mmap;
        while (vma) {
            vsize += vma->vm_end - vma->vm_start;
            vma = vma->vm_next;
        }
        printk(KERN_INFO "Virtual address space size for PID %d is %lu 
bytes\n", pid, vsize);
    } else {
        printk(KERN_INFO "No mm_struct available for PID %d\n", pid);
    }

    return 0;
}

static void __exit va_space_exit(void) {
    printk(KERN_INFO "Exiting VA space module\n");
}

module_init(va_space_init);
module_exit(va_space_exit);

