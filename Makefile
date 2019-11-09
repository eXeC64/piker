ARMGNU = /bin/arm-none-eabi
ARMGNUVER = 9.2.0

# source files
SOURCES_ASM := $(shell find src/ -type f -name '*.s')
SOURCES_C   := $(shell find src/ -type f -name '*.c')
 
# object files
OBJS := $(patsubst %.s,%.o,$(SOURCES_ASM))
OBJS += $(patsubst %.c,%.o,$(SOURCES_C))

all: kernel.img

kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img

kernel.elf: $(OBJS) linker.ld
	$(ARMGNU)-ld $(OBJS) /usr/lib/gcc/arm-none-eabi/$(ARMGNUVER)/libgcc.a -T linker.ld -o $@

%.o: %.c
	$(ARMGNU)-gcc -Wall -Werror -Wno-pointer-sign --std=c99 -c $< -I include -o $@

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

run: clean kernel.img
	qemu-system-arm -cpu arm1176 -M raspi2 -gdb tcp::1234 -m 512 -kernel kernel.img -serial stdio

debug: clean kernel.img
	qemu-system-arm -S -cpu arm1176 -M raspi2 -gdb tcp::1234 -m 512 -kernel kernel.img -serial stdio

gdb:
	$(ARMGNU)-gdb -q -ex "target remote :1234" -ex "layout reg" -ex "symbol-file kernel.elf" -ex 'set $$pc=0x10000'
 
.PHONY: clean tty qemu gdb
