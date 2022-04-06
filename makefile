CC=gcc
LD=ld
AS=as

out.elf: test-elf
	./$^

test-elf: elf.c
	$(CC) -o $@ $^ -I../src

code: code.o
	$(LD) $^ -o $@

%.o: %.S
	$(AS) $^ -o $@