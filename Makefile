FILES = ./build/kernel.o ./build/kernel.c.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc 

all: ./bin/boot.bin ./bin/kernel.bin $(FILES)
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >>./bin/os.bin


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
./build/kernel.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.o

./build/kernel.c.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.c.o
#...........kernel.asm...........




# Clean up intermediate and output files
clean:
	rm -rf ./build/*.o ./bin/*.bin
#./bin/boot.bin ./bin/boot.tmp ./build/*.o

