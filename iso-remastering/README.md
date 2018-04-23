iso_remastering.sh generic script, which modifies a Debian Live-Cd successfully, while using the following procedure:

    - create separate folder for the task and enter it
    - extract the image to the current location (I use bsdtar)
    - use unsquashfs to unpack the live/filesystem.squashfs file to a new folder
    - mount dev/pts to the unpacked filesystem
    - copy /etc/resolv.conf if you need network access (for installing packages etc)
    - chroot into the new folder with unpacked filesystem
    - mount /proc and /sys to this folder
    - change what is required and/or install/remove packages
    - exit the chroot environment when finished
    - unmount /proc /sys /dev/pts and remove resolv.conf from new folder
    - make a new filesystem.squashfs file (using mksquashfs) and overwrite the old one with it
    - create new iso image using xorriso

This is just an outline of the procedure and it was tested on a Debian 9.4.0 Live (Gnome) Stretch image.

The script is kept in local space... It works for different hosts then Debian, it was tested on Fedora 27 (kernel 4.15.16-300.fc27.x86_64), albeit, the Debian root tree, unsquashed, works like a charm, all the apt upgrade and other commands (deleting libreoffice and similar) work seamlessly using underline Fedora 27 4.15.16-300.fc27.x86_64 kernel.

Regarding the last command of the iso-remastering.sh script:

    sudo xorriso -as mkisofs -R -r -J -joliet-long -partition_offset 16 -A "Debian Gnome Live (DT)" -publisher "_nobody_" -V "Debian 9.4.0 Stretch" -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table -o debian-live-9.4.0-amd64-gnome.iso tmp_dir

It appeared that this line creates debian-live-9.4.0-amd64-gnome.iso, but bootable ONLY as legacy boot (using GRUB 1.99, and the isolinux/menu.cfg).

It is possible to change/re-shuffle order in the execution of GRUB options (and put first in the order which is Debian Installer), and it is possible to throw-in the timer (let say, for 5 seconds), after which the leading option - Debian Installer will boot automatically.

Here the shuffled menu.cfg is shown, and the first few lines of it:

    INCLUDE stdmenu.cfg
    MENU title Main Menu

    DEFAULT Debian Installer
    LABEL Debian Installer
    SAY "Booting Debian Installer..."
    linux /d-i/vmlinuz
    APPEND initrd=/d-i/initrd.gz 

    LABEL Debian GNU/Linux Live (kernel 4.9.0-6-amd64)
    SAY "Booting Debian GNU/Linux Live (kernel 4.9.0-6-amd64)..."
    linux /live/vmlinuz-4.9.0-6-amd64
    APPEND initrd=/live/initrd.img-4.9.0-6-amd64 boot=live components_

Actually, to be precise, GRUB 1.99 boots isolinux.cfg file rather then menu.cfg (menu.cfg is just a part of the isolinux.cfg boot menu sequence)!
