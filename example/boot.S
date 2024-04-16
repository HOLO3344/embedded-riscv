.macro long_load r, s
    j 1f
    .balign 8
2: .dword \s
1: la \r, 2b
#if __riscv_xlen == 32
    lw \r, 0(\r)
#else
    ld \r, 0(\r)
#endif
.endm

.section ".text.init"
.globl _start

_start:
    // Disable Interrupt
    csrrwi t0, mstatus, 0

    li   x1, 0
    li   x2, 0
    li   x3, 0
    li   x4, 0
    li   x5, 0
    li   x6, 0
    li   x7, 0
    li   x8, 0
    li   x9, 0
    li   x10, 0
    li   x11, 0
    li   x12, 0
    li   x13, 0
    li   x14, 0
    li   x15, 0
    li   x16, 0
    li   x17, 0
    li   x18, 0
    li   x19, 0
    li   x20, 0
    li   x21, 0
    li   x22, 0
    li   x23, 0
    li   x24, 0
    li   x25, 0
    li   x26, 0
    li   x27, 0
    li   x28, 0
    li   x29, 0
    li   x30, 0
    li   x31, 0

.option push
.option norelax
    long_load gp, __global_pointer$
    long_load sp, __init_sp
.option pop

    call main
