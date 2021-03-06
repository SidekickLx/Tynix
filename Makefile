#!Makefile
#
# --------------------------------------------------------
#
#    Tynix 这个小内核的 Makefile
#    默认使用的C语言编译器是 GCC、汇编语言编译器是 nasm
#
# --------------------------------------------------------
#

# patsubst 处理所有在 C_SOURCES 字列中的字（一列文件名），如果它的 结尾是 '.c'，就用 '.o' 把 '.c' 取代
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs



all: $(S_OBJECTS) $(C_OBJECTS) link update_iso

# The automatic variable `$<' is just the first prerequisite
.c.o:
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo linking...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o tx_kernel

test: $(S_OBJECTS) $(C_OBJECTS) link

iso: $(S_OBJECTS) $(C_OBJECTS) link update_iso

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) tx_kernel

.PHONY:update_iso
update_iso:
	cp tx_kernel isofiles/boot
	grub-mkrescue -o os.iso isofiles

.PHONY:qemu
qemu:
	qemu-system-i386 -cdrom os.iso -boot a

.PHONY:debug
debug:
	qemu -S -s -cdrom os.iso -boot a &
	sleep 1
	cgdb -x scripts/gdbinit

