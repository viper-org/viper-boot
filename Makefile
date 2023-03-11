AS=nasm
ASFLAGS=-fbin -I./inc

SRCS = src/stage1/boot.asm

TARGET=viperBoot.bin

$(TARGET): $(SRCS)
	$(AS) $(ASFLAGS) $^ -o $@