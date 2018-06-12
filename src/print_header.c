# define _GNU_SOURCE
#include <elf.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct_opt.h"

static void print_class(uint8_t ei_class)
{
	printf("  Class:\t\t\t\t");
	if (ei_class == ELFCLASS32)
		printf("ELF32\n");
	else if (ei_class == ELFCLASS64)
		printf("ELF64\n");
	else if (ei_class == ELFCLASSNONE)
		printf("Invalid class\n");
	else
		err(1, "print_header.c: print_class(); could not read EI_CLASS byte");
}

static void print_data(uint8_t ei_data)
{
	printf("  Data:\t\t\t\t\t");
	if (ei_data == ELFDATA2LSB)
		printf("2's complement, little-endian\n");
	else if (ei_data == ELFDATA2MSB)
		printf("2's complement, big-endian\n");
	else if (ei_data == ELFDATANONE)
		printf("Unknown data format\n");
	else
		err(1, "print_header.c: print_data(); could not read EI_DATA byte");
}

static void print_version(uint8_t ei_version)
{
	printf("  Version:\t\t\t\t");
	if (ei_version == EV_CURRENT)
		printf("%d (current)\n", ei_version);
	else if (ei_version == EV_NONE)
		printf("Invalid version\n");
	else
		err(1, "print_header.c: print_version(); could not read EI_VERSION byte");
}

static void print_osabi(uint8_t ei_osabi)
{
	printf("  OS/ABI:\t\t\t\t");
	if (ei_osabi == ELFOSABI_NONE || ei_osabi == ELFOSABI_SYSV)
		puts("UNIX - System V");
	else if (ei_osabi == ELFOSABI_HPUX)
		puts("HP-UX ABI.");
	else if (ei_osabi == ELFOSABI_NETBSD)
		puts("NetBSD ABI.");
	else if (ei_osabi == ELFOSABI_LINUX)
		puts("Linux ABI.");
	else if (ei_osabi == ELFOSABI_SOLARIS)
		puts("Solaris ABI.");
	else if (ei_osabi == ELFOSABI_IRIX)
		puts("IRIX ABI.");
	else if (ei_osabi == ELFOSABI_FREEBSD)
		puts("FreeBSD ABI.");
	else if (ei_osabi == ELFOSABI_TRU64)
		puts("TRU64 UNIX ABI.");
	else if (ei_osabi == ELFOSABI_ARM)
		puts("ARM architecture ABI.");
	else if (ei_osabi == ELFOSABI_STANDALONE)
		puts("Stand-alone (embedded) ABI.");
	else
		err(1, "print_osabi(); could not read EI_OSABI byte");
}

void print_abiv(uint8_t ei_abiv)
{
	printf("  ABI Version:\t\t\t\t%d\n", ei_abiv);
}

void print_type(uint16_t e_type)
{
	printf("  Type:\t\t\t\t\t");
	if (e_type == ET_NONE)
		puts("An unknown type.");
	else if (e_type == ET_REL)
		puts("A relocatable file.");
	else if (e_type == ET_EXEC)
		puts("EXEC (Executable file)");
	else if (e_type == ET_DYN)
		puts("A shared object.");
	else if (e_type == ET_CORE)
		puts("A core file.");
	else
		err(1, "print_type(); could not read e_type bytes");
}

void print_machine(uint16_t e_machine)
{
	printf("  Machine:\t\t\t\t");
	if (e_machine == EM_NONE)
		puts("An unknown machine.");
	else if (e_machine == EM_M32 )
		puts("AT&T WE 32100.");
	else if (e_machine == EM_SPARC)
		puts("Sun Microsystems SPARC.");
	else if (e_machine == EM_386)
		puts("Intel 80386.");
	else if (e_machine == EM_68K)
		puts("Motorola 68000.");
	else if (e_machine == EM_88K)
		puts("Motorola 88000.");
	else if (e_machine == EM_860)
		puts("Intel 80860.");
	else if (e_machine == EM_MIPS)
		puts("MIPS RS3000 (big-endian only).");
	else if (e_machine == EM_PARISC)
		puts("HP/PA.");
	else if (e_machine == EM_SPARC32PLUS)
		puts("SPARC with enhanced instruction set.");
	else if (e_machine == EM_PPC)
		puts("PowerPC.");
	else if (e_machine == EM_PPC64)
		puts("PowerPC 64-bit.");
	else if (e_machine == EM_S390)
		puts("IBM S/390");
	else if (e_machine == EM_ARM)
		puts("Advanced RISC Machines");
	else if (e_machine == EM_SH)
		puts("Renesas SuperH");
	else if (e_machine == EM_SPARCV9)
		puts("SPARC v9 64-bit.");
	else if (e_machine == EM_IA_64)
		puts("Intel Itanium");
	else if (e_machine == EM_X86_64)
		puts("Advanced Micro Devices x86-64");
	else if (e_machine == EM_VAX)
		puts("DEC Vax.");
	else
		err(1, "print_machine(); could not read e_machine bytes");
}


void print_version2(uint32_t e_version)
{
	printf("  Version:\t\t\t\t");
	if (e_version == EV_CURRENT)
		printf("0x%01x\n", e_version);
	else if (e_version == EV_NONE)
		printf("Invalid version\n");
	else
		err(1, "print_version2(); could not read EI_VERSION byte");
}

void print_entry(Elf64_Addr e_entry)
{
	printf("  Entry point address:\t\t\t0x%lx\n", e_entry);
}

void print_start_header(char *header_type, Elf64_Off e_offset)
{
	printf("  Start of %s headers:\t\t%ld (bytes into file)\n",
			header_type, e_offset);
}

void print_header_size(char *header_type, uint16_t h_size)
{
	char *s = strdup("s:");
	if (!strcmp(header_type, "this"))
	{
		s[0] = ':';
		s[1] = '\t';
	}
	printf("  Size of %s header%s\t\t%d (bytes)\n",
			header_type, s, h_size);
	free(s);
	s = NULL;
}

void print_number_header(char *header_type, uint16_t nb_h)
{
	printf("  Number of %s headers:\t\t%d\n", header_type, nb_h);
}

int print_header(void *e_map)
{
	if (!e_map)
		return 0;
	Elf64_Ehdr *ehdr = e_map;
	printf("ELF Header:\n  Magic:   ");
	for (int i = 0; i < EI_NIDENT; i++)
		printf("%02x ", ehdr->e_ident[i]);
	puts("");
	print_class(ehdr->e_ident[EI_CLASS]);
	print_data(ehdr->e_ident[EI_DATA]);
	print_version(ehdr->e_ident[EI_VERSION]);
	print_osabi(ehdr->e_ident[EI_OSABI]);
	print_abiv(ehdr->e_ident[EI_ABIVERSION]);
	print_type(ehdr->e_type);
	print_machine(ehdr->e_machine);
	print_version2(ehdr->e_version);
	print_entry(ehdr->e_entry);
	print_start_header("program", ehdr->e_phoff);
	print_start_header("section", ehdr->e_shoff);
	printf("  Flags:\t\t\t\t 0x%01x\n", ehdr->e_flags);
	print_header_size("this", ehdr->e_ehsize);
	print_header_size("program", ehdr->e_phentsize);
	print_number_header("program", ehdr->e_phnum);
	print_header_size("section", ehdr->e_shentsize);
	print_number_header("section", ehdr->e_shnum);
	printf("  Section header string table index:\t%d\n",
			ehdr->e_shstrndx);
	return 1;
}
