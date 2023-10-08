FILES =./build/prog1.o 
INCLUDES =-I../stdlib/src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc 


all:${FILES}
#nasm -f elf ./prog1.asm -o ./build/prog1.o
	i686-elf-gcc -g -T ./linker.ld -o ./prog1.elf -ffreestanding -O0 -nostdlib -fpic -g ${FILES} ../stdlib/stdlib.elf

./build/prog1.o: ./prog1.c
	i686-elf-gcc ${INCLUDES} -I./ $(FLAGS) -std=gnu99 -c ./prog1.c -o ./build/prog1.o


clean:
	rm -rf ${FILES}
	rm ./prog1.elf