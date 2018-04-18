#!/bin/bash
## For Debian host distro the following apt install should be done!
## sudo apt install bsdtar isolinux squashfs-tools syslinux syslinux-utils xorriso

## For Fedora host distro the following dnf install should be done!
## sudo dnf install bsdtar squashfs-tools syslinux xorriso

mkdir tmp_dir
cd tmp_dir/
sudo bsdtar xfp ../debian-live-9.4.0-amd64-gnome.iso.genesis
sudo unsquashfs -d work_dir live/filesystem.squashfs
sudo mount --bind /dev/pts/ work_dir/dev/pts/
sudo cp /etc/resolv.conf work_dir/etc/
sudo chroot work_dir/

#### Now in chroot environment ####
#### You can also change any other settings/options you prefer for your image ####

mount -t proc /proc /proc
mount -t sysfs /sys /sys
apt update
apt --purge autoremove libreoffice*
apt --purge autoremove quodlibet
apt install bash-completion
apt install gparted
apt upgrade (optional, was not required for my install as it was a recent image)
exit

#### Now into your normal environment ####

sudo umount ../tmp_dir/work_dir/sys
sudo umount ../tmp_dir/work_dir/proc
sudo umount ../tmp_dir/work_dir/dev/pts
sudo rm work_dir/etc/resolv.conf
sudo mksquashfs work_dir filesystem.squashfs
sudo mv filesystem.squashfs live/filesystem.squashfs
sudo mv work_dir/ ../
cd ../
sudo xorriso -as mkisofs -R -r -J -joliet-long -partition_offset 16 -A "Debian Gnome Live (DT)" -publisher "_nobody_" -V "Debian 9.4.0 Stretch" -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -o debian-live-9.4.0-amd64-gnome.iso tmp_dir
