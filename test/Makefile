CC=x86_64-elf-gcc
CFLAGS=-nostdlib -ffreestanding -I. -O2

AS=nasm
ASFLAGS=-felf64

LD=x86_64-elf-ld
LDFLAGS=-nostdlib -Tlink.ld -zmax-page-size=0x1000

CSRCS=$(shell find . -name '*.c')
ASSRCS=$(shell find . -name '*.asm')
OBJS:=${CSRCS:.c=.o} ${ASSRCS:.asm=.o}

KERNEL=kernel.elf
TARGET=fat.img

.PHONY: all loader
all: $(TARGET)

loader:
	make -C ..

ovmf:
	mkdir -p $@
	cd ovmf && curl -Lo OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(TARGET): $(KERNEL) loader
	cp ../BOOTX64.EFI ../build ./
	mkdir -p boot
	cp ./viper.cfg testmod.txt $< boot/
	./build boot

.PHONY: run clean distclean
run: $(TARGET) ovmf
	qemu-system-x86_64 -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk

clean:
	rm -rf $(OBJS) $(KERNEL) $(TARGET) boot/

distclean:
	rm -rf ovmf