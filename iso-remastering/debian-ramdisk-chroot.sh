## debian-ramdisk-chroot.sh explanation

## All files in /real-root are in the "real" root, the things in "/" are in a simulated
## (RAM disk) root. When writing in the simulated root, anything writen is stored in a
## RAM disk (tmpfs). It will perish upon rebooting.
##
## Alternatively, "chroot" command should be used to switch to /real-root temporarily.
## The following should be done also: mount /proc/, /sys, /dev, /dev/pts, so that more
## commands work in the "real" root. It also bind-mounts /etc/resolv.conf, to use
## network commands while in the realroot.

#!/bin/sh
su -m
mount --bind /dev/pts/ /real-root/dev/pts/
cp /etc/resolv.conf /real-root/etc/resolv.conf
chroot /real-root/
mount -t proc /proc /proc
mount -t sysfs /sys /sys

## If command cp /etc/resolv.conf /real-root/etc/resolv.conf is not executed:
echo "nameserver IP_addr1" > /etc/resolv.conf
echo "nameserver IP_addr2" >> /etc/resolv.conf

apt update
apt upgrade
apt-get install rt-tools

## Execute what is needed after
