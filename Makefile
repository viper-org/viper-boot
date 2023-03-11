AS=nasm
ASFLAGS=-fbin -I./inc

SRCS=src/stage1/boot.asm src/stage2/stage2.asm

BINS=${SRCS:.asm=.bin}

TARGET=viperBoot.bin

all: $(TARGET)

%.bin: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(BINS)
	cat $^ > $@

clean:
	rm -f $(BINS) $(TARGET)

run: $(TARGET)
	qemu-system-x86_64 -hda $(TARGET)