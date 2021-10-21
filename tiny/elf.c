#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <fcntl.h>

/**
* http://www.sco.com/developers/gabi/2000-07-17/ch4.eheader.html
* http://www.sco.com/developers/gabi/2000-07-17/ch5.pheader.html
* http://www.sco.com/developers/gabi/latest/ch4.intro.html
* http://www.sco.com/developers/gabi/1998-04-29/ch4.sheader.html
* http://www.sco.com/developers/gabi/1998-04-29/ch4.strtab.html
**/

int main(void) {
	
	const char text[] = {
		"Hello World!\n"
		// write "hello world!\n" to stdout
		"\x48\xC7\xC0\x01\x00\x00\x00"	// mov rax, 1
		"\x48\x89\xC7"					// mov rdi, rax
		"\x48\xC7\xC6\x78\x00\x40\x00"	// mov rsi, message(0x0000000000400078)
		"\x48\xC7\xC2\x0D\x00\x00\x00"	// mov rdx, 13
		"\x0F\x05"						// syscall
		// exit
		"\x48\xC7\xC0\x3C\x00\x00\x00"	// mov rax, 60
		"\x48\xC7\xC7\x00\x00\x00\x00"	// mov rdi, 0
		"\x0F\x05"						// syscall
	};

	Elf64_Phdr programheaders[1] = {
		{
			.p_type   =	PT_LOAD,
			.p_flags  =	PF_R | PF_X,
			.p_offset =	0x0000000000000078,
			.p_vaddr  =	0x0000000000400078,
			.p_paddr  =	0x0000000000400000,
			.p_filesz =	sizeof(text),
			.p_memsz  =	sizeof(text),
			.p_align  =	0x1000
		}
	};


	Elf64_Ehdr header = {
		.e_ident = {
			0x7F, 'E', 'L', 'F', // ELF MAGIC
			0x02, // CLASS: 64bit
			0x01, // DATA: little endian
			0x01, // ELF Version: the one and only
			0x00, // OS ABI: System V
			0x00, // EI_ABIVERSION (ignored by the linux kernel)
			// the rest is currently unused
		},
		.e_type = ET_EXEC,
		.e_machine = EM_X86_64,
		.e_version = EV_CURRENT,
		.e_entry = 0x400085,
		.e_phoff = sizeof(Elf64_Ehdr),
		.e_shoff = 0,
		.e_flags = 0,
		.e_ehsize = sizeof(Elf64_Ehdr),
		.e_phentsize = sizeof(Elf64_Phdr),
		.e_phnum = 1,
		.e_shentsize = 0,
		.e_shnum = 0,
		.e_shstrndx = 0
	};

	char program[0xb0] = {};
	// write ELF header
	memcpy(program, &header, sizeof(header));
	// write program headers
	memcpy(program+header.e_phoff, programheaders, sizeof(programheaders));
	// write .text
	memcpy(program+programheaders[0].p_offset, text, sizeof(text));


	// write program to file
	FILE* out = fopen("out.elf", "wb");
	fwrite(program, 1, sizeof(program), out);
	fclose(out);

	return 0;
}