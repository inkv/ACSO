all: clean floppy.img 

clean:
	rm -f *.o *.bin *.elf *.img

floppy.img: clean kernel.bin boot.bin
		cat boot.bin kernel.bin /dev/zero | dd bs=512 count=2880 of=floppy.img

loader.o: loader.asm
		nasm -f elf loader.asm -o loader.o

kernel.bin: kernel.elf
		objcopy -R .note -R .comment -S -O binary kernel.elf kernel.bin
		
cmd.o:
		gcc -m32 -ffreestanding -fno-builtin -nostdlib -c 

kernel.elf: loader.o kmain.c 
		gcc -m32  -ffreestanding -fno-builtin -nostdlib -c kmain.c
		ld -melf_i386 -Ttext 0x1000 -o kernel.elf loader.o kmain.o 

boot.bin: boot.asm
		nasm -f bin boot.asm -o boot.bin

test: floppy.img
		qemu-system-i386 -boot a -fda floppy.img -curses