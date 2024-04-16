#!/bin/bash

set -e

CURDIR=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
QEMU_SRC_DIR=${CURDIR}/../qemu
QEMU_BUILD_DIR=${CURDIR}/qemu_build

<<!
if [ ! -f ${QEMU_BUILD_DIR} ]
then
    mkdir -p ${QEMU_BUILD_DIR}
fi
!

rm -rf ${QEMU_BUILD_DIR}
mkdir -p ${QEMU_BUILD_DIR}

cd ${QEMU_BUILD_DIR} && \
${QEMU_SRC_DIR}/configure --target-list=riscv64-softmmu --enable-debug-tcg \
--enable-debug --enable-debug-info -enable-trace-backends=log && \
make -j8
