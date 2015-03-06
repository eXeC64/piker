ARMGNU ?= /bin/arm-none-eabi
ARMGNUVER ?= 4.9.3

CFLAGS ?= -Wall -Werror -Wno-pointer-sign --std=c99 -g

# source files
SOURCES_ASM := $(shell find src/ -type f -name '*.s')
SOURCES_C   := $(shell find src/ -type f -name '*.c')
 
# object files
OBJS        := $(patsubst %.s,%.o,$(SOURCES_ASM))
OBJS        += $(patsubst %.c,%.o,$(SOURCES_C))

all: kernel.img

include $(wildcard *.d)

kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img

kernel.elf: $(OBJS) linker.ld
	$(ARMGNU)-ld $(OBJS) /usr/lib/gcc/arm-none-eabi/$(ARMGNUVER)/libgcc.a -T linker.ld -o $@

%.o: %.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -I include -o $@

%.o: %.s
	$(ARMGNU)-as $< -o $@

clean:
	rm -f $(OBJS) kernel.elf kernel.img
 
#for uart, the pins should be connected as such:
#.RBWG........
#.............
#SD card slot is on the left, usb ports on the right
tty: kernel.img
	../raspbootin/raspbootcom/raspbootcom /dev/ttyUSB0 kernel.img

qemu: kernel.img
	qemu-system-arm -cpu arm1176 -gdb tcp::1234 -nographic -S -m 512M -kernel ./kernel.img -serial stdio

gdb:
	$(ARMGNU)-gdb -ex "target remote :1234" -ex "layout reg" -ex "symbol-file kernel.elf"
 

.PHONY: clean tty qemu gdb
