#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>

static struct dentry *dir = 0;
static u32 hello = 0;

static int debugs_init(void)
{
  struct dentry *junk;
  // Create directory /sys/kernel/debug/test
  dir = debugfs_create_dir("test", 0);
  if (!dir) {
    printk(KERN_ALERT "debugfs_test: failed to create /sys/kernel/debug/test\n");
    return -1;
  }

  // Create file:echo 0 > /sys/kernel/debug/test/hello
  junk = debugfs_create_u32("hello", 0666, dir, &hello);
  if (!junk) {
    printk(KERN_ALERT "debugfs_test: failed to create /sys/kernel/debug/test/hello\n");
    return -1;
  }
  return 0;
}

static void debugs_exit(void)
{
  debugfs_remove_recursive(dir);
}
module_init(debugs_init);
module_exit(debugs_exit);
MODULE_LICENSE("GPL");
