#!/bin/bash

CURDIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
QEMU=${CURDIR}/qemu_build/qemu-system-riscv64

#QEMU=/home/holo/work/gitee/learn-linux/work-dir/qemu_build/qemu-system-riscv64
IMAGE=${CURDIR}/example/example.elf

LOG=
#LOG+="-d in_asm,op,guest_errors,unimp -D ./qemu_log_`date +%Y%m%d%H%M`.log"
#LOG+="-d in_asm,guest_errors,unimp -D ./qemu_log_`date +%Y%m%d%H%M`.log"


GDB=

if [ $# == 1 ] && [ $1 == "gdb" ]
then
    GDB+="-gdb tcp::4567 -S"
fi

${QEMU} \
    -m 2G \
    -smp 1 \
    -nographic \
    -echr 0x14 \
    -machine virt \
    -bios ${IMAGE} ${GDB} ${LOG}

