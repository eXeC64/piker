ARMGNU ?= /opt/arm-2008q3/bin/arm-none-eabi

# source files
SOURCES_ASM := $(wildcard src/*.s)
SOURCES_C   := $(wildcard src/*.c)
 
# object files
OBJS        := $(patsubst %.s,%.o,$(SOURCES_ASM))
OBJS        += $(patsubst %.c,%.o,$(SOURCES_C))

all: kernel.img

include $(wildcard *.d)

kernel.elf: $(OBJS) linker.ld
	$(ARMGNU)-ld $(OBJS) /opt/arm-2008q3/lib/gcc/arm-none-eabi/4.3.2/libgcc.a -T linker.ld -o $@

kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img

clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
 
dist-clean: clean
	$(RM) -f *.d
 
%.o: %.c
	$(ARMGNU)-gcc -Wall -Werror --std=c99 -c $< -I include -o $@
 
%.o: %.s
	$(ARMGNU)-as $< -o $@


