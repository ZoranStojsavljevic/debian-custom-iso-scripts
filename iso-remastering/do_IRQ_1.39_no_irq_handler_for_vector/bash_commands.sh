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

## Serial port blocking command on receive:
cat < /dev/ttyS0

## Serial port unblocking command for sending
echo "test" > /dev/ttyS0

## Pointer for irqbalance program: https://github.com/Irqbalance/irqbalance
_______

root@arm:/home/debian# stty -F /dev/ttyUSB0 19200
stty: /dev/ttyUSB0: No such file or directory
root@arm:/home/debian# setserial -a /dev/ttySC0
/dev/ttySC0, Line 0, UART: undefined, Port: 0x0000, IRQ: 76
	Baud_base: 1500000, close_delay: 50, divisor: 0
	closing_wait: 3000
	Flags: spd_normal low_latency

root@arm:/home/debian# stty -F /dev/ttySC0 115200
root@arm:/home/debian# setserial -a /dev/ttySC0
/dev/ttySC0, Line 0, UART: undefined, Port: 0x0000, IRQ: 76
	Baud_base: 1500000, close_delay: 50, divisor: 0
	closing_wait: 3000
	Flags: spd_normal low_latency

root@arm:/home/debian# setserial -G /dev/ttySC0
/dev/ttySC0 uart undefined port 0x0000 irq 76 baud_base 1500000 spd_normal low_latency
root@arm:/home/debian# setserial -a /ev/ttySC0
