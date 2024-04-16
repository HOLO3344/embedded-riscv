// See LICENSE for license details.
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "print.h"
#include "common.h"
#include "drv_uart.h"

static void putch(char ch);
static inline void printnum(unsigned long long num, unsigned int base);
static void format_string(const char *fmt, va_list ap);

static void putch(char ch)
{
    drv_uart_send_data((uint8_t *)(&ch), 1);
}

/* Writes number to putchar. */
static inline void printnum(unsigned long long num, unsigned int base)
{
    unsigned digs[sizeof(num) * 8];
    int pos = 0;

    for (;;) {
        digs[pos++] = num % base;
        if (num < base) {
            break;
        }
        num /= base;
    }

    while (pos-- > 0) {
        putch(digs[pos] + (digs[pos] >= 10 ? 'a' - 10 : '0'));
    }
}

/* Format and print a string. */
static void format_string(const char *fmt, va_list ap)
{
    const char* p;
    const char* last_fmt;
    char ch;
    unsigned int num;

    while (1) {
        while ((ch = *(unsigned char *) fmt) != '%') {
            putch(ch);
            if (ch == '\0') {
                return;
            }
            fmt++;
        }
        fmt++;

        // Process a %-escape sequence
        last_fmt = fmt;

        switch (ch = *(unsigned char *)fmt++) {
            /* character */
            case 'c':
                putch(va_arg(ap, int));
                break;

            /* string */
            case 's':
                if ((p = va_arg(ap, char *)) == NULL) {
                    p = "(null)";
                }

                for (; (ch = *p) != '\0';) {
                    putch(ch);
                    p++;
                }
                break;

            /* (signed) decimal */
            case 'd':
                num = va_arg(ap, unsigned int);
                printnum(num, 10);
                break;

            /* no break, fall through to hexidecimal */
            /* (unsigned) hexadecimal */
            case 'x':
            case 'X':
                num = va_arg(ap, unsigned int);
                printnum(num, 16);
                break;

                /* escaped '%' character */
            case '%':
                putch(ch);
                break;

                /* unrecognized escape sequence */
            default:
                putch('%');
                fmt = last_fmt;
                break;
        }
    }
}

int print(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    format_string(fmt, ap);

    va_end(ap);

    return 0;
}

