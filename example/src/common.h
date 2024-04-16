#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#define READ_REG32(addr)            (*(volatile unsigned int *)(addr))
#define WRITE_REG32(addr, data)     READ_REG32(addr) = (data)

#define UNUSED(x)                   (void)(x)

#endif

