
all: ./bin/boot.bin

./bin/boot.bin: ./bin/boot.tmp
	cat $^ > $@
	rm -f $^

./bin/boot.tmp: ./build/boot.o ./build/gdt.o ./build/disk.o
	cat $^ > $@

./build/boot.o: ./src/AUB_Bootloader/boot.asm
	nasm -f bin -o $@ $<

./build/gdt.o: ./src/AUB_Bootloader/gdt.asm
	nasm -f bin -o $@ $<

./build/disk.o: ./src/AUB_Bootloader/disk.asm
	nasm -f bin -o $@ $<

# Clean up intermediate and output files
clean:
	rm -f ./bin/boot.bin ./bin/boot.tmp ./build/*.o

