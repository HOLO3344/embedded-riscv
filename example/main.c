#include "log.h"
#include "drv_uart.h"

int main(void)
{
    int ret = 0;

    drv_uart_init();

    LOGD("Build Date: %s, Build Time: %s\r\n", __DATE__, __TIME__);

    while (1) {
        ;
    }

    // unreachable
    return ret;
}
