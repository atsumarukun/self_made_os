#!/bin/bash

/files/edk2/bootloader_build.sh
cd /files/programs/kernel
make all clean
/files/qemu/run_qemu.sh