## bash_commands.sh - some useful commands to test do_IRQ: 1.39 No…irq handler for vector issue!

## Command to find all active IOAPIC interrupts using /sys file system
find . -name irq -print -exec cat {} \;

## Browse INT (IO-APIC and PCI-MSI domains)
cat /proc/interrupts

## Find the mapings of the two IOAPIC interrupts to the vector number
cd /sys/kernel/debug/irq/irqs
/sys/kernel/debug/irq/irqs# grep 39 *

## Command to start Graphical Desktop Manager (GDM)
systemctl start getty@tty7

## Command to stop Graphical Desktop Manager (GDM)
systemctl stop getty@tty7
