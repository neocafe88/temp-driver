/*
 * aesdchar.h
 *
 *  Created on: Oct 23, 2019
 *      Author: Dan Walkes
 */

#include <linux/mutex.h>

#ifndef TEMP_DRIVER_TEMPDRIVER_H_
#define TEMP_DRIVER_TEMPDRIVER_H_

#define TEMP_DEBUG 1  
// (above) Remove comment on this line to enable debug

#undef PDEBUG             /* undef it, just in case */
#ifdef TEMP_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "tempdriver: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

struct temp_dev
{
    // lock
    struct mutex lock;

    // character device structure
    struct cdev cdev;     
};

#endif /* TEMP_DRIVER_TEMPDRIVER_H_ */
