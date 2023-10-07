before running, make sure to set environment
that is you can do .build.sh
then run command, like make, make all etc.

----------------
in gdb,
you can use
add-symbol-file ./build/kernelfull.o 0x100000
then,yes

break _start
target remote | qemu-system-x86_64 -S -gdb stdio -hda ./bin/os.bin
......................
target remote | qemu-system-x86_64 -hda ./bin/os.bin -gdb stdio -S
break kernel_main

//(gdb) target remote | qemu-system-i386 -hda ./bin/os.bin -S -gdb stdio
//(gdb) add-symbol-file ./build/kernelfull.o 0x100000

layout asm

........................
To see, elf configuration.
dumpelf ./prog1.elf
readelf -a ./prog1.elf
 readelf -S ./prog
  readelf -s ./prog