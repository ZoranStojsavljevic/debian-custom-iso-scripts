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

The script is kept in local space... It works for different hosts then Debian, it was tested on Fedora 27 (kernel 4.15.16-300.fc27.x86_64), albeit, the Debian root tree, unsquashed, works like a charm, all the apt upgrade and other commands (deleting libreoffice and similar) work seamlessly using undernine Fedora 27 4.15.16-300.fc27.x86_64 kernel.
