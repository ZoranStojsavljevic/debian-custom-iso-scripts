## Command to find all active IOAPIC interrupts using /sys file system
find . -name irq -print -exec cat {} \;

## Command to start Graphical Desktop Manager (GDM)
systemctl start getty@tty7

## Command to stop Graphical Desktop Manager (GDM)
systemctl stop getty@tty7
