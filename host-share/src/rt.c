#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/cred.h>
#include <linux/fs.h>

#define DEBUG_ENABLED 1

/* MODULE EXIT/INIT */
static int __init rk_init(void) {
    #ifdef DEBUG_ENABLED
        printk(KERN_INFO "Initializing module");
    #endif
    return 0;
}

static int __exit rk_exit(void) {
    #ifdef DEBUG_ENABLED
        printk(KERN_INFO "Cleanup module");
    #endif
    return 0;
}
