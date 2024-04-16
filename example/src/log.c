#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include "drv_uart.h"

#define DUMP_SIZE_PER_LINE      (16)

#if (ENABLE_HEX_DUMP)
void dump_hex(uint8_t *addr, uint32_t len, const char *desc)
{
    uint32_t i;

    PRINT_FUNC("\r\nHex dump[%s]", desc);

    for (i = 0; i < len; i++) {
        if (0 == (i % DUMP_SIZE_PER_LINE))
        {
            PRINT_FUNC("\r\n");
        }

        PRINT_FUNC("%x ", addr[i]);
    }

    PRINT_FUNC("\r\n");
}
#else
void dump_hex(uint8_t *addr, uint32_t len, const char *desc)
{
    (void)(addr);
    (void)(len);
    (void)(desc);
}
#endif

