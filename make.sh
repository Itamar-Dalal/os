#!/usr/bin/bash

export PATH=$HOME/opt/cross/bin:$PATH
cd src
if ! make; then
    echo "Build failed"
    exit 1
fi

cp kernel.bin ../iso/kernel/kernel.bin
make clean
cd ..

while ! mkisofs -R -input-charset utf8 -b boot/grub/stage2_eltorito -boot-info-table -no-emul-boot -boot-load-size 4 -o os.iso iso; do
    echo "ISO creation failed. Retrying..."
done
echo "ISO creation succeeded."

while ! qemu-system-x86_64 -cdrom os.iso; do
    echo "QEMU failed. Retrying..."
done
echo "QEMU execution succeeded."
