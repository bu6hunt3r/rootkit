#define DEBUG_ENABLED 1
#define MAX_PIDS 512
#define DEVICE_NAME "ttyR0"
#define CLASS_NAME "ttyR"

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED == 1
    # define DEBUG(...)		printk(KERN_INFO __VA_ARGS__)
#else
    # define DEBUG(...)
#endif