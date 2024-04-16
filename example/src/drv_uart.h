#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <stdint.h>

void drv_uart_init(void);
void drv_uart_send_data(const uint8_t *p_data, uint32_t size);

#endif

