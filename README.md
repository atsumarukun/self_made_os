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
<br><br>

# 作成開始日
2022/09/14