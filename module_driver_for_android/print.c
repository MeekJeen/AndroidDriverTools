#include <linux/init.h>
#include <linux/module.h>

static int printk_init(void)
{
    printk(KERN_EMERG "Printk Level 0 !\n");
    printk(KERN_ALERT "Printk Level 1 !\n");
    printk(KERN_CRIT "Printk Level 2 !\n");
    printk(KERN_ERR "Printk Level 3 !\n");
    printk(KERN_WARNING "Printk Level 4 !\n");
    printk(KERN_NOTICE "Printk Level 5 !\n");
    printk(KERN_INFO "Printk Level 6 !\n");
    printk("<7>" "Printk Level 7 !\n");

    return 0;
}

static void printk_exit(void)
{
    printk(KERN_EMERG "Printk Level 0 !\n");
    printk(KERN_ALERT "Printk Level 1 !\n");
    printk(KERN_CRIT "Printk Level 2 !\n");
    printk(KERN_ERR "Printk Level 3 !\n");
    printk(KERN_WARNING "Printk Level 4 !\n");
    printk(KERN_NOTICE "Printk Level 5 !\n");
    printk(KERN_INFO "Printk Level 6 !\n");
    printk(KERN_DEBUG "Printk Level 7 !\n");
}

module_init(printk_init);
module_exit(printk_exit);
MODULE_LICENSE("GPL");

