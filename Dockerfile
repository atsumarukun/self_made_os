FROM ubuntu:20.04

RUN apt update && \
    apt -y install tzdata
ENV TZ=Asia/Tokyo

RUN apt update && \
    apt -y install uuid-runtime && \
    apt -y install qemu-system-x86 qemu-utils lld clang nasm python3-distutils llvm-dev build-essential dosfstools