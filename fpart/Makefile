include ../../toolchain.mk

CFLAGS=-m32 -c -nostdlib --freestanding -I$(LIBPIP)/include/ -I$(LIBPIP)/arch/x86/include/ -fno-stack-protector -fno-pic -fno-pie
ASFLAGS=$(CFLAGS)
LDFLAGS=-L$(LIBPIP)/lib -melf_i386 -Tlink.ld -lpip

ASSOURCES=$(wildcard *.s)
CSOURCES=$(wildcard *.c)

ASOBJ=$(ASSOURCES:.s=.o)
COBJ=$(CSOURCES:.c=.o)

EXEC=fpart.bin

all: $(EXEC)
	@echo Done.

clean:
	rm -fv $(ASOBJ) $(COBJ) $(EXEC)

$(EXEC): $(ASOBJ) $(COBJ)
	$(LD) $^ -o $@ $(LDFLAGS)
	cp $(EXEC) ../images/.

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
