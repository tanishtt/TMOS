all:
	nasm -f elf ./prog1.asm -o ./build/prog1.o
	i686-elf-gcc -g -T ./linker.ld -o ./prog1.bin -ffreestanding -O0 -nostdlib -fpic -g ./build/prog1.o


clean:
	rm -rf ./build/prog1.o