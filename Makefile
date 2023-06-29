# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

$(eval $(call DEFAULT_VAR,CC,cc))
$(eval $(call DEFAULT_VAR,LD,ld))
$(eval $(call DEFAULT_VAR,OBJCOPY,objcopy))

CFLAGS ?= -g -O0 -pipe -Wall -Wextra
LDFLAGS ?=

override LDFLAGS +=                        \
    -Tlimine-efi/gnuefi/elf_x86_64_efi.lds \
    -nostdlib                              \
    -z max-page-size=0x1000                \
    -m elf_x86_64                          \
    -static                                \
    -pie                                   \
    --no-dynamic-linker                    \
    -z text

override CFLAGS +=          \
    -std=gnu11              \
    -ffreestanding          \
    -fno-stack-protector    \
    -fno-stack-check        \
    -fshort-wchar           \
    -fno-lto                \
    -fpie                   \
    -m64                    \
    -march=x86-64           \
    -mabi=sysv              \
    -mno-80387              \
    -mno-mmx                \
    -mno-sse                \
    -mno-sse2               \
    -mno-red-zone

override CPPFLAGS :=        \
    -Iinclude               \
    -Ilimine-efi/inc        \
    -Ilimine-efi/inc/x86_64 \
    $(CPPFLAGS)             \
    -DGNU_EFI_USE_MS_ABI    \
    -MMD

override CFILES := $(shell find ./src -type f -name '*.c')
override OBJ := $(CFILES:.c=.o)
override HEADER_DEPS := $(CFILES:.c=.d)

override ELF := viperBoot.elf
override TARGET := BOOTX64.EFI
override BUILD := build

.PHONY: all
all: $(TARGET) $(BUILD)

limine-efi:
	git clone https://github.com/limine-bootloader/limine-efi.git

limine-efi/gnuefi/crt0-efi-x86_64.S.o limine-efi/gnuefi/reloc_x86_64.c.o: limine-efi
	$(MAKE) -C limine-efi/gnuefi ARCH=x86_64

$(BUILD): build.cpp
	g++ -std=c++20 $< -o $@

$(TARGET): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): limine-efi/gnuefi/crt0-efi-x86_64.S.o limine-efi/gnuefi/reloc_x86_64.c.o $(OBJ)
	$(LD) $^ $(LDFLAGS) -o $@

-include $(HEADER_DEPS)
%.o: %.c limine-efi
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(ELF) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -rf limine-efi
