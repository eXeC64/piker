PREFIX ?= /opt/gcc-rpi
ARMGNU ?= $(PREFIX)/bin/arm-none-eabi

CFLAGS ?= -Wall -Werror -Wno-pointer-sign --std=c99

# source files
SOURCES_ASM := $(shell find src/ -type f -name '*.s')
SOURCES_C   := $(shell find src/ -type f -name '*.c')
 
# object files
OBJS        := $(patsubst %.s,%.o,$(SOURCES_ASM))
OBJS        += $(patsubst %.c,%.o,$(SOURCES_C))

all: kernel.img

include $(wildcard *.d)

kernel.elf: $(OBJS) linker.ld
	$(ARMGNU)-ld $(OBJS) $(PREFIX)/lib/gcc/arm-none-eabi/4.7.4/libgcc.a -T linker.ld -o $@

kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img

clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
 
dist-clean: clean
	$(RM) -f *.d

run:
	../raspbootin/raspbootcom/raspbootcom /dev/ttyUSB0 kernel.img
 
%.o: %.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -I include -o $@
 
%.o: %.s
	$(ARMGNU)-as $< -o $@


.PHONY: clean dist-clean run
