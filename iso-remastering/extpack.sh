#!/bin/bash
## Download the latest VBox extpack!
## LatestVirtualBoxVersion=$(wget -qO - http://download.virtualbox.org/virtualbox/LATEST.TXT) && wget "http://download.virtualbox.org/virtualbox/${LatestVirtualBoxVersion}/Oracle_VM_VirtualBox_Extension_Pack-${LatestVirtualBoxVersion}.vbox-extpack"

VBoxExtpack=$(VBoxManage list extpacks | grep Version: | awk '{print $2}')
echo $VBoxExtpack

VBoxVersion=$(sudo dnf list installed | grep virtualbox | awk '{print $2}'| sed 's/_/ /g'| awk '{print $1}')
echo $VBoxVersion

if [ "$VBoxExtpack" != "$VBoxVersion" ]; then
        echo "VBox components NOT in sync: VBoxExtpack=$VBoxExtpack; VBoxVersion=$VBoxVersion!"
        exit 127
else echo "VBox components in sync: VBoxExtpack=$VBoxExtpack = VBoxVersion=$VBoxVersion!"
fi

## You might want to verify its integrity by comparing its SHA-256 checksum available in file:
## SHA256SUMS=wget "https://www.virtualbox.org/download/hashes/${LatestVirtualBoxVersion}/SHA256SUMS"
## echo "SHA256SUM is $SHA256SUMS"

## Then, extpack could be installed as follows:
## sudo VBoxManage extpack install --replace Oracle_VM_VirtualBox_Extension_Pack-${LatestVirtualBoxVersion}.vbox-extpack

## To verify if it has been successfully installed, please, list the installed extension packs:
VBoxManage list extpacks

## To uninstall the extension pack:
## sudo VBoxManage extpack uninstall "Oracle VM VirtualBox Extension Pack"
