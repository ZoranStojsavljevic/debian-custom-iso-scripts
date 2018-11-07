### Issue do_IRQ: 1.39 No irq handler for vector described here:
```
  https://bugzilla.kernel.org/show_bug.cgi?id=201617
  https://bugzilla.kernel.org/show_bug.cgi?id=201617#c6
```
Zoran 2018-11-06 15:24:42 UTC
Hello,

Today, I have tried to test more extensively this issue. To do this, I
did the following:
[A] Disabled /dev/ttyS0 (I/O 0x3F8, irq 4) using BIOS;
[B] Decided NOT to use stress-ng, just to try to use serial /dev/ttyS1 commands;
[C] Play with CLI commands, using simple bash online commands and scripts;
[D] The same customer BIOS used (MCU 0x23).

While using customer Linux dlvt83 4.18.10 (Thu October 18, 2018.)
x86_64 image, the following is true:
The issue is easy to reproduce using the following commands:
# stty -F /dev/ttyS1 (one message 50/50 - roughly every second time)
Changing baud rate on fly with # stty -F /dev/ttyS1 1200 to stty -F
/dev/ttyS1 576000 does not change anything in timing and occurrence
frequency;
Executing online script # while true; do "ABC" > /dev/ttyS1; sleep 10;
done& introduces in 80% cases IRQ message, only single one after 10
sec. (sleep time interval);
The irq 3 is mapped to vector 39, BUT ONLY on one local LAPIC (CPU1
irq3 LAPIC 100% count in my case, CPU0 irq3 LAPIC is rock solid 0%
count);

While using Linux Debian 4.9.0/7 - amd64 Debian 4.9.110-1
(2018-07-05), the following is true:
The issue is NOT reproducible at all (with above commands and scripts.
also from previous testing with stress-ng);
The irq 3 is visible on both local LAPICs (CPU0 irq3 LAPIC had 80%
count of the interrupts in my case, CPU1 irq3 LAPIC around 20% count);
_______

This prompted me to start thinking that some patch is changing
delivery in IOAPIC mode, I/O APIC domain to LAPICs to be
deterministic, so there is no race effect in between which core should
take INT handling. Obvious change.

So, either:
[1] APIC MADT table is all wrong from BIOS (but Debian distro from
early July 2018. works);
[2] Some patch in kernel, patching IOAPIC mode (from late July 2018.)
delivers this blow!

Please, do read https://wiki.osdev.org/IOAPIC

It seems that IOREDTBL [delivery mode] register was changed, from low
priority delivery mode (001) to fixed priority delivery mode (000).

Delivery Mode (8 - 10) How the interrupt will be sent to the CPU(s).
It can be 000 (Fixed), 001 (Lowest Priority), 010 (SMI), 100 (NMI),
101 (INIT) and 111 (ExtINT). Most of the cases you want Fixed mode, or
Lowest Priority if you don't want to suspend a high priority task on
some important Processor/Core/Thread.

All comments and suggestions are very welcome!

Thank you,
Zoran
