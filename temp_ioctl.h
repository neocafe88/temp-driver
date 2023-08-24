/*
 * temp_ioctl.h
 *
 *  Created on: Aug 24, 2023 
 *      Author: Hyoun Cho
 *
 *  It's been updated from the original file used for the course work
 *  orignally written by Prof. Dan Walkes
 *
 *  @brief Definitins for the ioctl used on temperature sensor devices
 */

#ifndef TEMP_IOCTL_H
#define TEMP_IOCTL_H

#ifdef __KERNEL__
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#endif

/**
 * A structure to be passed by IOCTL from user space to kernel space, describing the type
 * of seek performed on the temp driver
 */
struct temp_seekto {
    /**
     * The zero referenced write command to seek into
     */
    uint32_t write_cmd;
    /**
     * The zero referenced offset within the write
     */
    uint32_t write_cmd_offset;
};

// Pick an arbitrary unused value from https://github.com/torvalds/linux/blob/master/Documentation/userspace-api/ioctl/ioctl-number.rst
#define TEMP_IOC_MAGIC 0x16

// Define a write command from the user point of view, use command number 1
#define TEMPDRIVER_IOCSEEKTO _IOWR(TEMP_IOC_MAGIC, 1, struct temp_seekto)
/**
 * The maximum number of commands supported, used for bounds checking
 */
#define TEMPDRIVER_IOC_MAXNR 1

#endif /* TEMP_IOCTL_H */
