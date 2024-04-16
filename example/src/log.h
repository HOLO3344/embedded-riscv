#ifndef __LOG_H__
#define __LOG_H__

#include <stdint.h>
#include <stdio.h>
#include "print.h"

#define LOG_ERROR_LEVEL     (0)
#define LOG_WARN_LEVEL      (1)
#define LOG_INFO_LEVEL      (2)
#define LOG_DBUG_LEVEL      (3)

#ifndef LOG_LEVEL
#define LOG_LEVEL           (LOG_DBUG_LEVEL)
#endif

#ifndef ENABLE_HEX_DUMP
#define ENABLE_HEX_DUMP     (1)
#endif

#define PRINT_FUNC      print

#if (LOG_LEVEL >= LOG_ERROR_LEVEL)
#define LOGE(fmt, ...)  PRINT_FUNC("[Error]:[%s]" fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOGE(...)
#endif

#if (LOG_LEVEL >= LOG_WARN_LEVEL)
#define LOGW(fmt, ...)  PRINT_FUNC("[Warning]:[%s]" fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOGW(...)
#endif

#if (LOG_LEVEL >= LOG_INFO_LEVEL)
#define LOGI(fmt, ...)  PRINT_FUNC("[Info]:[%s]" fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOGI(...)
#endif

#if (LOG_LEVEL >= LOG_DBUG_LEVEL)
#define LOGD(fmt, ...)  PRINT_FUNC("[Debug]:[%s]" fmt "\n", __func__, ##__VA_ARGS__)
#else
#define LOGD(...)
#endif

void dump_hex(uint8_t *addr, uint32_t len, const char *desc);

#endif /* __LOG_H__ */

