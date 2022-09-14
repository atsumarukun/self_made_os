DISK_IMG=disk.img
BASE_DIR=/files/qemu
MOUNT_POINT=$BASE_DIR/mnt

if [ ! -d $MOUNT_POINT ] 
then
    mkdir $MOUNT_POINT
fi

rm -f $BASE_DIR/$DISK_IMG

qemu-img create -f raw $BASE_DIR/$DISK_IMG 200M
mkfs.fat -n 'Self made OS' -s 2 -f 2 -R 32 -F 32 $BASE_DIR/$DISK_IMG

mount -o loop $BASE_DIR/$DISK_IMG $MOUNT_POINT
mkdir -p $MOUNT_POINT/EFI/BOOT

cp /files/edk2/Build/LoaderPkgX64/DEBUG_CLANG38/X64/Loader.efi $MOUNT_POINT/EFI/BOOT/BOOTX64.EFI
umount $MOUNT_POINT

qemu-system-x86_64 \
    -m 1G \
    -drive if=pflash,format=raw,file=$BASE_DIR/OVMF_CODE.fd \
    -drive if=pflash,format=raw,file=$BASE_DIR/OVMF_VARS.fd \
    -drive if=ide,index=0,media=disk,format=raw,file=$BASE_DIR/$DISK_IMG \
    -device nec-usb-xhci,id=xhci \
    -device usb-mouse \
    -device usb-kbd \
    -monitor stdio $QEMU_OPTS

rm -r $BASE_DIR/$DISK_IMG $MOUNT_POINT