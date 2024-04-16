CROSS_COMPILE ?= ../riscv64-unknown-elf-toolchain-10.2.0-2020.12.8-x86_64-linux-ubuntu14/bin/riscv64-unknown-elf-
TARGET ?= example.elf
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
READELF = $(CROSS_COMPILE)readelf
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

FILTER_OUT_FILES :=

LDFLAGS := -T./linker.ld

CFLAGS := -ffreestanding -mcmodel=medany -Wall -Werror -ffunction-sections -fdata-sections $(DEFINE)
CFLAGS += -O2

ASMFLAGS := -mcmodel=medany -Wall -Werror $(DEFINE)
ASMFLAGS += -O2

ALL_DIRS := $(shell ls -R | grep '^\./*.*:$$' | awk '{gsub(":",""); print}')
C_SOURCES := $(filter-out $(FILTER_OUT_FILES), $(foreach n, $(ALL_DIRS), $(wildcard $(n)/*.c)))
ASM_SOURCES := $(foreach n, $(ALL_DIRS), $(wildcard $(n)/*.S))
INCLUDE_PATH := $(foreach n, $(ALL_DIRS), $(addprefix -I, $(n)))
C_OBJ_FILES := $(patsubst %.c, %.o, $(C_SOURCES))
ASM_OBJ_FILES := $(patsubst %.S, %.o, $(ASM_SOURCES))
OBJDUMP_ASM := $(subst .o,.o.asm, $(C_OBJ_FILES))
OBJDUMP_ASM += $(subst .o,.o.asm, $(ASM_OBJ_FILES))

LDFLAGS += --gc-sections

.PHONY: show clean spike boot_rom download_image

boot_rom:clean $(TARGET)
	@echo "Build boot rom....."

download_image: clean $(TARGET)
	@echo "Build Download Image....."

$(TARGET):$(C_OBJ_FILES) $(ASM_OBJ_FILES) $(ASM_OBJ_FILES_AFTER_C)
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJDUMP) -xDS $@ > $@.dump
	$(OBJCOPY) -O binary $@ $@.bin
	$(READELF) -a $@ > $@.info
	$(SIZE) $@

%.o:%.c
	$(CC) -c $^ $(CFLAGS) $(INCLUDE_PATH) -o $@
	$(OBJDUMP) -xDS $@ > $@.asm

%.o:%.S
	$(CC) -c $^ $(ASMFLAGS) $(INCLUDE_PATH) -o $@
	$(OBJDUMP) -xDS $@ > $@.asm

show:
	@echo "ALL_DIRS:"
	@echo $(ALL_DIRS)
	@echo
	@echo "C_SOURCES:"
	@echo $(C_SOURCES)
	@echo
	@echo "C_OBJ_FILES:"
	@echo $(C_OBJ_FILES)
	@echo
	@echo "ASM_SOURCES:"
	@echo $(ASM_SOURCES)
	@echo
	@echo "ASM_OBJ_FILES:"
	@echo $(ASM_OBJ_FILES)
	@echo
	@echo "INCLUDE_PATH:"
	@echo $(INCLUDE_PATH)
	@echo

spike:
	~/work/adi-2/phase2-repo/output/spike/bin/spike -d $(TARGET)

clean:
	-rm $(C_OBJ_FILES) $(ASM_OBJ_FILES) $(OBJDUMP_ASM) *.elf *.dump *.bin *.info

