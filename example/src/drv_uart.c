/* SPDX-License-Identifier: GPL-2.0-only */

#include <stdint.h>
#include "common.h"

/*
 * These are the definitions for the Line Control Register
 *
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
*/

#define UART_LCR_WLS_MSK    0x03       /* character length select mask */
#define UART_LCR_WLS_5      0x00        /* 5 bit character length */
#define UART_LCR_WLS_6      0x01        /* 6 bit character length */
#define UART_LCR_WLS_7      0x02        /* 7 bit character length */
#define UART_LCR_WLS_8      0x03        /* 8 bit character length */
#define UART_LCR_STB        0x04        /* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN        0x08        /* Parity eneble */
#define UART_LCR_EPS        0x10        /* Even Parity Select */
#define UART_LCR_STKP       0x20        /* Stick Parity */
#define UART_LCR_SBRK       0x40        /* Set Break */
#define UART_LCR_DLAB       0x80        /* Divisor latch access bit */

/*
 * These are the definitions for the Line Status Register
*/

#define UART_LSR_DR     0x01        /* Data ready */
#define UART_LSR_OE     0x02        /* Overrun */
#define UART_LSR_PE     0x04        /* Parity error */
#define UART_LSR_FE     0x08        /* Framing error */
#define UART_LSR_BI     0x10        /* Break */
#define UART_LSR_THRE   0x20        /* Xmit holding register empty */
#define UART_LSR_TEMT   0x40        /* Xmitter empty */
#define UART_LSR_ERR    0x80        /* Error */

#define SINGLE_CHAR_TIMEOUT  10000
#define FIFO_TIMEOUT         10000

typedef unsigned char reg_t;

typedef struct dw16550_uart
{
    /* offset: 0x00 */
    union {
        __IOM reg_t rbr;
        __IOM reg_t thr;
        __IOM reg_t dll;
    };

    /* offset: 0x04 */
    union {
        __IOM reg_t ier;
        __IOM reg_t dlh;
    };

    /* offset: 0x08 */
    union {
        __IOM reg_t iir;
        __IOM reg_t fcr;
    };

    /* offset: 0x0c */
    __IOM reg_t lcr;

    /* offset: 0x10 */
    __IOM reg_t mcr;

    /* offset: 0x14 */
    __IOM reg_t lsr;

    /* offset: 0x18 */
    __IOM reg_t msr;

    /* offset: 0x1c */
    __IOM reg_t scr;

    __IM  reg_t reseverd[40];

    /* offset: 0xc0 */
} dw16550_uart_t;

static inline void *uart_platform_baseptr(void)
{
    return (void *)(0x10000000);
}

static unsigned int uart_baudrate_divisor(unsigned int baudrate,
    unsigned int refclk, unsigned int oversample,
    unsigned int *rem)
{
    if (rem) {
        *rem = refclk % (baudrate * oversample);
    }

    return refclk / (baudrate * oversample);
}
static void dw16550_setbaud(struct dw16550_uart *regs,
        unsigned int baud_divisor,
        unsigned int baud_divisor_rem)
{
    regs->lcr |= UART_LCR_DLAB;
    regs->dll = baud_divisor & 0xFF;
    regs->dlh = (baud_divisor >> 8) & 0xFF;
    regs->lcr &= ~UART_LCR_DLAB;
}

static void dw16550_uart_init(struct dw16550_uart *regs,
        unsigned int divisor, unsigned int divisor_rem)
{
    while (!(regs->lsr & UART_LSR_TEMT)) {
        ;
    }

    regs->ier = 0x00;       /* Disable all interrupt */
    regs->lcr = 0x03;       /* 8 data, 1 stop, no parity */
    regs->fcr = 0x07;       /* Enable RX/TX FIFO and reset RX/TX FIFO */
    regs->mcr = 0x03;       /* enable RTS,DTR 2'b11 */

    dw16550_setbaud(regs, divisor, divisor_rem);
}

static int dw16550_uart_can_tx_byte(struct dw16550_uart *regs)
{
    return (regs->lsr & UART_LSR_THRE);
}

static void dw16550_uart_tx_byte(struct dw16550_uart *regs,
        unsigned char data)
{
    volatile unsigned long int i = SINGLE_CHAR_TIMEOUT;

    while (i-- && !dw16550_uart_can_tx_byte(regs)) {
        asm volatile("nop");
    }

    // Send data
    regs->thr = data;
}

static void dw16550_uart_tx_flush(struct dw16550_uart *regs)
{
    volatile unsigned long int i = FIFO_TIMEOUT;

    while (i-- && !dw16550_uart_can_tx_byte(regs)) {
        asm volatile("nop");
    }
}

void drv_uart_init(void)
{
    struct dw16550_uart *regs = uart_platform_baseptr();

    if (!regs) {
        return;
    }

    unsigned int rem;
    unsigned int div = uart_baudrate_divisor(115200, 50000000, 16, &rem);

    dw16550_uart_init(regs, div, rem);
}

void uart_tx_byte(unsigned char data)
{
    struct dw16550_uart *regs = uart_platform_baseptr();

    if (!regs) {
        return;
    }

    dw16550_uart_tx_byte(regs, data);
}

void drv_uart_send_data(const uint8_t *p_data, uint32_t size)
{
    while (size--) {
        uart_tx_byte(*p_data++);
    }
}

unsigned char uart_rx_byte(void)
{
    struct dw16550_uart *regs = uart_platform_baseptr();

    if (!regs) {
        return 0xff;
    }

    return 0x00;
}

void uart_tx_flush(void)
{
    struct dw16550_uart *regs = uart_platform_baseptr();

    if (!regs) {
        return;
    }

    dw16550_uart_tx_flush(regs);
}
