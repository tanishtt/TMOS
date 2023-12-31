FILES = ./build/kernel.o ./build/kernel.c.o ./build/loader/formats/elf.o ./build/loader/formats/elfloader.o ./build/disk/disk.o ./build/disk/streamer.o ./build/task/tss.asm.o ./build/task/task.o ./build/task/task.asm.o ./build/task/process.o ./build/isr80h/isr80h.o ./build/isr80h/heap.o ./build/isr80h/process.o ./build/keyboard/keyboard.o ./build/keyboard/classic.o ./build/isr80h/misc.o ./build/isr80h/io.o ./build/fs/pparser.o ./build/fs/file.o ./build/fs/fat/fat16.o ./build/string/string.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/gdt/gdt.o ./build/gdt/gdt.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.o ./build/memory/paging/paging.asm.o 
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc 
#MAKE = make

all: ./bin/boot.bin ./bin/kernel.bin user_programs
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >>./bin/os.bin
#1mb block size means 16mb of data.
	sudo mount -t vfat ./bin/os.bin /mnt/d
#copy a file over
	sudo cp ./hello.txt /mnt/d
#sudo cp ./test.txt /mnt/d
	sudo cp ./programs/prog1/prog1.elf /mnt/d
	sudo cp ./programs/shell/shell.elf /mnt/d
#at run time it will mount and unmount both
	sudo umount /mnt/d


./bin/kernel.bin : $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o 



#..................bootloader.bin..................
./bin/boot.bin: ./bin/boot.tmp
	cat $^ > $@
	rm -f $^

./bin/boot.tmp: ./build/boot.o ./build/gdt.o ./build/disk.o
	cat ./build/boot.o > $@

./build/boot.o: ./src/AUB_Bootloader/boot.asm
	nasm -f bin -o $@ $<

./build/gdt.o: ./src/AUB_Bootloader/gdt.asm
	nasm -f bin -o $@ $<

./build/disk.o: ./src/AUB_Bootloader/disk.asm
	nasm -f bin -o $@ $<
#...................bootloader ends....................
#.....gdt

./build/gdt/gdt.o: ./src/gdt/gdt.c
	i686-elf-gcc $(INCLUDES) -I./src/gdt $(FLAGS) -std=gnu99 -c ./src/gdt/gdt.c -o ./build/gdt/gdt.o

./build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o


#.....gdt ends here
#.....idt

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o:./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o


#.....idt end here.
#.....io

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

#.....io ends here
#.....heap

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
		i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o


#.....heap ends here
#.....kheap starts here.

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
		i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

#.....kheap ends here.
#.....paging starts here.

./build/memory/paging/paging.o: ./src/memory/paging/paging.c
		i686-elf-gcc $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

#.....paging ends here.
#.....kernel
./build/kernel.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.o

./build/kernel.c.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.c.o
#...........kernel.asm...........
#.....disk starts here in c
./build/disk/disk.o: ./src/disk/disk.c
	i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/disk/streamer.o: ./src/disk/streamer.c
	i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o


#.....disk ends here
#.....fs starts here

./build/fs/pparser.o: ./src/fs/pparser.c
		i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o

./build/fs/file.o: ./src/fs/file.c
		i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/file.c -o ./build/fs/file.o

./build/fs/fat/fat16.o: ./src/fs/fat/fat16.c
		i686-elf-gcc $(INCLUDES) -I./src/fs -I./src/fs/fat $(FLAGS) -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o


#.....fs ends here.
#.....string

./build/string/string.o: ./src/string/string.c
		i686-elf-gcc $(INCLUDES) -I./src/string $(FLAGS) -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

#.....string ends.
#.....tasks

./build/task/tss.asm.o: ./src/task/tss.asm
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o

./build/task/task.o: ./src/task/task.c
	i686-elf-gcc $(INCLUDES) -I./src/task $(FLAGS) -std=gnu99 -c ./src/task/task.c -o ./build/task/task.o

./build/task/process.o: ./src/task/process.c
	i686-elf-gcc $(INCLUDES) -I./src/task $(FLAGS) -std=gnu99 -c ./src/task/process.c -o ./build/task/process.o

./build/task/task.asm.o: ./src/task/task.asm
	nasm -f elf -g ./src/task/task.asm -o ./build/task/task.asm.o


#.....tasks ends here.
#.....programs

user_programs:
	cd ./programs/stdlib && $(MAKE) all
	cd ./programs/prog1 && $(MAKE) all
	cd ./programs/shell && $(MAKE) all


user_programs_clean:
	cd ./programs/stdlib && $(MAKE) clean
	cd ./programs/prog1 && $(MAKE) clean
	cd ./programs/shell && $(MAKE) clean

#.....programs ends here.
#.....isr80h

./build/isr80h/isr80h.o: ./src/isr80h/isr80h.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/isr80h.c -o ./build/isr80h/isr80h.o

./build/isr80h/misc.o: ./src/isr80h/misc.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/misc.c -o ./build/isr80h/misc.o

./build/isr80h/io.o: ./src/isr80h/io.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/io.c -o ./build/isr80h/io.o

./build/isr80h/heap.o: ./src/isr80h/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/heap.c -o ./build/isr80h/heap.o

./build/isr80h/process.o: ./src/isr80h/process.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/process.c -o ./build/isr80h/process.o


#.....isr80h ends here.
#.....keyboard.
./build/keyboard/keyboard.o: ./src/keyboard/keyboard.c
	i686-elf-gcc $(INCLUDES) -I./src/keyboard $(FLAGS) -std=gnu99 -c ./src/keyboard/keyboard.c -o ./build/keyboard/keyboard.o

./build/keyboard/classic.o: ./src/keyboard/classic.c
	i686-elf-gcc $(INCLUDES) -I./src/keyboard $(FLAGS) -std=gnu99 -c ./src/keyboard/classic.c -o ./build/keyboard/classic.o



#.....keyboard ends here.
#.....elf

./build/loader/formats/elfloader.o: ./src/loader/formats/elfloader.c
	i686-elf-gcc $(INCLUDES) -I./src/loader/formats $(FLAGS) -std=gnu99 -c ./src/loader/formats/elfloader.c -o ./build/loader/formats/elfloader.o

./build/loader/formats/elf.o: ./src/loader/formats/elf.c
	i686-elf-gcc $(INCLUDES) -I./src/loader/formats $(FLAGS) -std=gnu99 -c ./src/loader/formats/elf.c -o ./build/loader/formats/elf.o


#.....elf_ends_here.

# Clean up intermediate and output files
clean:user_programs_clean
	rm -Rf ./build/*.o ./bin/*.bin ./build/*.asm.o
#./bin/boot.bin ./bin/boot.tmp ./build/*.o

