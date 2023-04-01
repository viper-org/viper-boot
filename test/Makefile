AS=nasm
ASFLAGS=-felf64

LD=x86_64-elf-ld
LDFLAGS=-nostdlib

ASSRCS=$(shell find . -name '*.asm')
OBJS:=${ASSRCS:.asm=.o}

KERNEL=kernel.elf
TARGET=fat.img

.PHONY: all loader
all: $(TARGET)

loader:
	make -C ..

ovmf:
	mkdir -p $@
	cd ovmf && curl -Lo OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(TARGET): $(KERNEL) loader
	cp ../BOOTX64.EFI ../build ./
	mkdir -p boot
	cp ./viper.cfg $< boot/
	./build boot

.PHONY: run clean distclean
run: $(TARGET) ovmf
	qemu-system-x86_64 -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk

clean:
	rm $(OBJS) $(KERNEL) $(TARGET)

distclean:
	rm -rf ovmf