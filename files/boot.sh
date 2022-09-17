#!/bin/bash

cd /files/programs/kernel
make all clean
/files/edk2/bootloader_build.sh
/files/qemu/run_qemu.sh