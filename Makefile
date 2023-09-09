FILES = ./build/kernel.o

all: ./bin/boot.bin ./bin/kernel.bin $(FILES)
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >>./bin/os.bin


./bin/kernel.bin : $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o 



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




./build/kernel.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.o

#...........kernel.asm...........




# Clean up intermediate and output files
clean:
	rm -rf ./build/*.o ./bin/*.bin
#./bin/boot.bin ./bin/boot.tmp ./build/*.o

