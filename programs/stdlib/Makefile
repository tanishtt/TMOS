FILES =./build/start.o ./build/function.o
INCLUDES =

all:${FILES}
	i686-elf-ld -m elf_i386 -relocatable ${FILES} -o ./stdlib.elf

./build/start.o: ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.o

./build/function.o: ./src/function.asm
	nasm -f elf ./src/function.asm -o ./build/function.o


clean:
	rm -rf ${FILES}
	rm ./stdlib.elf