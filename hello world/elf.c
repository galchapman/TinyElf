#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int main(void) {
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
		.e_entry = 0x401000,
		.e_phoff = sizeof(Elf64_Ehdr),
		.e_shoff = 8568,
		.e_flags = 0,
		.e_ehsize = sizeof(Elf64_Ehdr),
		.e_phentsize = sizeof(Elf64_Phdr),
		.e_phnum = 3,
		.e_shentsize = sizeof(Elf64_Shdr),
		.e_shnum = 6,
		.e_shstrndx = 5
	};
	Elf64_Phdr programheaders[3] = {
		{
			.p_type   =	PT_LOAD,
			.p_flags  =	PF_R,
			.p_offset =	0x0000000000000000,
			.p_vaddr  =	0x0000000000400000,
			.p_paddr  =	0x0000000000400000,
			.p_filesz =	0x00000000000000e8,
			.p_memsz  =	0x00000000000000e8,
			.p_align  =	0x1000
		},
		{
			.p_type   =	PT_LOAD,
			.p_flags  =	PF_R | PF_X,
			.p_offset =	0x0000000000001000,
			.p_vaddr  =	0x0000000000401000,
			.p_paddr  =	0x0000000000401000,
			.p_filesz =	0x0000000000000027,
			.p_memsz  =	0x0000000000000027,
			.p_align  =	0x1000
		},
		{
			.p_type   = PT_LOAD,
			.p_flags  =	PF_R | PF_W,
			.p_offset =	0x0000000000002000,
			.p_vaddr  =	0x0000000000402000,
			.p_paddr  =	0x0000000000402000,
			.p_filesz =	0x000000000000000e,
			.p_memsz  =	0x000000000000000e,
			.p_align  =	0x1000
		}
	};

	const char snames[] = "" "\0" ".text" "\0" ".data" "\0" ".symtab" "\0" ".strtab" "\0" ".shstrtab" "\0";

	Elf64_Shdr sections[6] = {
		{
			.sh_name	  = 0,
			.sh_type	  = SHT_NULL,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00000000,
			.sh_size	  = 0x0000000000000000,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 0,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .text
			.sh_name	  = 1,
			.sh_type	  = SHT_PROGBITS,
			.sh_flags	  = SHF_ALLOC | SHF_EXECINSTR,
			.sh_addr	  = 0x0000000000401000,
			.sh_offset	  = 00001000,
			.sh_size	  = 0x0000000000000027,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 16,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .data
			.sh_name	  = 7,
			.sh_type	  = SHT_PROGBITS,
			.sh_flags	  = SHF_ALLOC | SHF_WRITE,
			.sh_addr	  = 0x0000000000402000,
			.sh_offset	  = 0x00002000,
			.sh_size	  = 0x000000000000000e,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 4,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .symtab
			.sh_name	  = 13,
			.sh_type	  = SHT_SYMTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00002010,
			.sh_size	  = 0x0000000000000000,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 8,
			.sh_entsize	  = 0x0000000000000018
		},
		{ // .strtab
			.sh_name	  = 21,
			.sh_type	  = SHT_STRTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00002010,
			.sh_size	  = 0x0000000000000000,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 1,
			.sh_entsize	  = 0x0000000000000000
		},
		{ // .shstrtab
			.sh_name	  = 29,
			.sh_type	  = SHT_STRTAB,
			.sh_flags	  = 0,
			.sh_addr	  = 0x0000000000000000,
			.sh_offset	  = 0x00002010,
			.sh_size	  = 0x0000000000000027,
			.sh_link	  = 0,
			.sh_info	  = 0,
			.sh_addralign = 1,
			.sh_entsize	  = 0x0000000000000000
		}
	};

	const char text[] = {
		"\xB8\x01\x00\x00\x00\xBF\x01\x00\x00\x00\x48\xBE\x00\x20\x40\x00\x00\x00\x00\x00\xBA\x0E\x00\x00\x00\x0F\x05\xB8\x3C\x00\x00\x00\xBF\x00\x00\x00\x00\x0F\x05"
	};

	const char data[] = {
		"Hello World!\n"
	};

	char program[8952] = {};
	// write ELF header
	memcpy(program, &header, sizeof(header));
	// write program headers
	memcpy(program+header.e_phoff, programheaders, sizeof(programheaders));
	// write sections
	memcpy(program+header.e_shoff, sections, sizeof(sections));
	// write sections names
	memcpy(program+sections[header.e_shstrndx].sh_offset, snames, sizeof(snames));
	// write .text
	memcpy(program+programheaders[1].p_offset, text, sizeof(text));
	// write .data
	memcpy(program+programheaders[2].p_offset, data, sizeof(data));


	// write program to file
	FILE* out = fopen("out.elf", "wb");
	fwrite(program, 1, sizeof(program), out);
	fclose(out);

	return 0;
}