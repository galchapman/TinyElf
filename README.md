# Tiny ELF

The smallest elf 64 program (I think)
It code is refrenced in code.asm
this code only contains the minimum amount of program headers(1) to run a program

## Run

```sh
$ make out.elf
$ ./out.elf
Hello World!
```

## Hex dump

```
00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
00000010  02 00 3e 00 01 00 00 00  86 00 40 00 00 00 00 00  |..>.......@.....|
00000020  40 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |@...............|
00000030  00 00 00 00 40 00 38 00  01 00 00 00 00 00 00 00  |....@.8.........|
00000040  01 00 00 00 05 00 00 00  78 00 00 00 00 00 00 00  |........x.......|
00000050  78 00 40 00 00 00 00 00  00 00 40 00 00 00 00 00  |x.@.......@.....|
00000060  27 00 00 00 00 00 00 00  27 00 00 00 00 00 00 00  |'.......'.......|
00000070  00 10 00 00 00 00 00 00  48 65 6c 6c 6f 2c 20 57  |........Hello, W|
00000080  6f 72 6c 64 21 0a 66 ff  c7 48 89 f8 48 c7 c6 78  |orld!.f..H..H..x|
00000090  00 40 00 b2 0e 0f 05 b0  3c 66 31 ff 0f 05        |.@......<f1...|
```

## Why I'm pretty sure it's the smallest

The first 64 bytes of the file are the elf header. It size is fixed (in ELF64), so every ELF file must have it.\
After it there are 56 bytes of program header.
The program header is reponsible for loading the code from the file to memory.\
After it is the text to print. and than the two syscalls.\
After all of that we are left with exactly 158 bytes of pure ELF File without any bloat embeded in it.
