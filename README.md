## edk2
UEFI規格のBootLoaderの作成に[edk2](https://github.com/tianocore/edk2)を利用する。
```sh
$ git clone https://github.com/tianocore/edk2.git
$ cd edk2
$ git checkout 38c8be123aced4cc8ad5c7e0da9121a181b94251
$ git submodule init
$ git submodule update
$ cd BaseTools/Source/C
$ make
```
<br>

## QEMU
エミュレータに[QEMU](https://www.qemu.org/)を利用する。
<br><br>

## Newlib
C++の標準ライブラリにNewlibを用いる。
```sh
PREFIX=/files/x86_64-elf
COMMON_CFLAGS="-nostdlibinc -O2 -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS"
CC=clang
CXX=clang++
TARGET_TRIPLE=x86_64-elf

git clone --depth 1 --branch fix-build https://github.com/uchan-nos/newlib-cygwin.git
mkdir build_newlib
cd build_newlib
../newlib-cygwin/newlib/configure CC=$CC CC_FOR_BUILD=$CC CFLAGS="-fPIC $COMMON_CFLAGS" --target=$TARGET_TRIPLE --prefix=$PREFIX --disable-multilib --disable-newlib-multithread
make -j 4
make install
```
<br><br>

## 実行
dockerコンテナを立ち上げると自動でコンパイル、ディスクの作成、エミュレータの起動を行う。
```sh
docker-compose up
```
<br>

## 作業環境
環境|バージョン
----|----
ArchLinux(x86_64)|5.18.9-arch1-1
Docker|20.10.17
Docker Compose|2.6.1
<br>

## 参考文献
- 内田公太(2021).『ゼロからのOS自作入門』.マイナビ
- osdev-jp(2017).「[EDK II で UEFI アプリケーションを作る](https://osdev-jp.readthedocs.io/ja/latest/2017/create-uefi-app-with-edk2.html)」.osdev-jp core(2022/09/14)
- Kazuki Ohara(2021).「[Mac で始める「ゼロからのOS自作入門」](https://qiita.com/yamoridon/items/4905765cc6e4f320c9b5)」.Qiita(2022/09/14)
- 「[ELF-64 Object File Format](https://uclibc.org/docs/elf-64-gen.pdf)」.ELF-64 Object File Format(2022/09/16)
- 「[osdev-jp](https://osdev.jp/wiki/building-libcxx)」.osdev-jp(2022/09/18)
- AMD(2021). 「[AMD64 Architecture Programmer’s Manual: Volumes 1-5](https://www.amd.com/system/files/TechDocs/40332.pdf)」.AMD(2022/09/21)
<br><br>

## 作成開始日
2022/09/14