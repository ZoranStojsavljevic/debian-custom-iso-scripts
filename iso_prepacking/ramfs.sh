### Phase 3: How to make preseed Debian Live distro for VBox VM automatic creation!
### Please, go to https://cdimage.debian.org/cdimage/weekly-live-builds/amd64/iso-hybrid/
### and download the latest debian-live-testing-amd64-gnome.iso image

#!/bin/bash
### Scripts are on this location: /data/vms/vbox/iso_files
cd /data/vms/vbox/iso_files

### Make the working image debian-live-testing-amd64-gnome.iso
cp debian-live-testing-amd64-gnome.iso.genesis debian-live-testing-amd64-gnome.iso

### Create the working directory: /data/vms/vbox/iso_files/try-buster
mkdir /data/vms/vbox/iso_files/try-buster

### Unpack the debian-live-testing-amd64-gnome.iso into try-buster/ directory
7z x -otry-buster/ debian-live-testing-amd64-gnome.iso

### Remove the default .iso image
rm debian-live-testing-amd64-gnome.iso

### copy initrd.gz to initrd.gz.genesis (intermediate testing step)
cd try-buster/d-i
cp initrd.gz initrd.gz.genesis

## How to unpack and repack an initial ramdisk (initrd/initramfs) image?
## https://access.redhat.com/solutions/24029
mkdir test/
mv initrd.gz test/
cd test/

### Unpack initrd.gz
zcat initrd.gz | cpio -idmv

### Cleanup: remove original initrd.gz
rm initrd.gz
cp /data/vms/vbox/iso_files/preseed.cfg .

### Create preseed initrd.gz
find . | cpio -o -c | gzip -9 > ../initrd.gz
cd ..

### Here, the cleanup should be performed (another testing step)
ls -al initrd.gz
ls -al test/ ## real command: rm -rf test/
pwd ## should be deleted

### Prepare to pack the presedeed .iso file
cd ..
xorriso -as mkisofs -U -r -v -T -J -joliet-long -V "Debian 10.0.0 Buster" -volset "Debian 10.0.0 Buster" -A "Debian 10.0.0 Buster" -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -eltorito-alt-boot -e boot/grub/efi.img -no-emul-boot -graft-points -isohybrid-mbr /usr/share/syslinux/isohdpfx.bin -o debian-live-testing-amd64-gnome.iso .
mv debian-live-testing-amd64-gnome.iso ..
