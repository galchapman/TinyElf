CC=gcc
LD=ld
AS=as

out.elf: build-elf
	./$^

build-elf: elf.c
	$(CC) -o $@ $^

code: code.o
	$(LD) $^ -o $@

%.o: %.S
	$(AS) $^ -o $@