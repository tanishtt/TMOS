all:
	nasm -f elf ./program1.asm -o ./build/program1.o
	i686-elf-gcc -g -T ./linker.ld -o ./program1.bin -ffreestanding -O0 -nostdlib -fpic -g ./build/program1.o


clean:
	rm -rf ./build/program1.o