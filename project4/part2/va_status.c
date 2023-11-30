#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>

static char *addr_str;
static int pid;

module_param(addr_str, charp, 0000);
MODULE_PARM_DESC(addr_str, "Virtual Address as a string");
module_param(pid, int, 0000);
MODULE_PARM_DESC(pid, "Process ID");

static int __init va_status_init(void) {
    struct task_struct *task;
    struct mm_struct *mm;
    unsigned long addr;
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;
    spinlock_t *ptl;

    printk(KERN_INFO "va_status: Module loaded.\n");

    if (!addr_str || kstrtoul(addr_str, 16, &addr)) {
        printk(KERN_INFO "va_status: Invalid address input.\n");
        return -EINVAL;
    }

    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        printk(KERN_INFO "va_status: Cannot find task with PID %d.\n", pid);
        return -ESRCH;
    }

    mm = task->mm;
    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) return -EINVAL;

    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud)) return -EINVAL;

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) return -EINVAL;

    ptep = pte_offset_map_lock(mm, pmd, addr, &ptl);
    if (!ptep) return -EINVAL;

    pte = *ptep;
    pte_unmap_unlock(ptep, ptl);

    if (pte_present(pte)) {
        printk(KERN_INFO "va_status: Address 0x%lx is in memory.\n", addr);
    } else {
        printk(KERN_INFO "va_status: Address 0x%lx is on disk.\n", addr);
    }

    return 0;
}

static void __exit va_status_exit(void) {
    printk(KERN_INFO "va_status: Module unloaded.\n");
}

module_init(va_status_init);
module_exit(va_status_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Reports the current status of a specific virtual address.");
