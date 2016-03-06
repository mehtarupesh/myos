AS:=i586-elf-as
CC:=i586-elf-gcc

CFLAGS:=-ffreestanding -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -g
CPPFLAGS:=
LIBS:=-lgcc

OBJS:=\
boot.o \
kernel.o \
terminal.o \
string.o \
putc.o \
printf.o \

OBJS_DIR:=build_files

all: myos.bin

.PHONEY: all clean iso run-qemu

myos.bin: $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS) $(LIBS)
	mkdir $(OBJS_DIR)
	mv $(OBJS) $(OBJS_DIR)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	$(AS) $< -o $@ -g

clean:
	rm -rf isodir
	rm -rf $(OBJS_DIR)
	rm -f myos.bin myos.iso myos.sym $(OBJS)

iso: myos.iso

isodir isodir/boot isodir/boot/grub:
	mkdir -p $@

isodir/boot/myos.bin: myos.bin isodir/boot
	cp $< $@

isodir/boot/grub/grub.cfg: grub.cfg isodir/boot/grub
	cp $< $@

myos.iso: isodir/boot/myos.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run-qemu: myos.iso
	qemu-system-i386 -cdrom myos.iso
