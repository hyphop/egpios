#!/bin/sh

#flags="-O3 -ffast-math -funroll-loops -Wall -fPIC -DPIC"
#flags="-O3 -ffast-math -funroll-loops -Wall -fPIC -DPIC -mfloat-abi=hard -mfpu=neon -march=armv7-a -mtune=cortex-a9"

export ARCH=arm
export CROSS_COMPILE=arm-openwrt-linux-
export bdir=$PWD
export STAGING_DIR=../toolchain-arm_cortex-a9+neon_gcc-5.3.0_musl-1.1.15_eabi

export PATH=$STAGING_DIR/bin/:$PATH

GCC=${CROSS_COMPILE}gcc

CFLAGS=$flags

echo $GCC $CFLAGS -o egpios egpios.c
$GCC $CFLAGS -o egpios egpios.c
${CROSS_COMPILE}strip egpios

#scp egpios root@zeru:/tmp