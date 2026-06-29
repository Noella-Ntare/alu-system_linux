#include "hnm.h"

/**
 * get_weak_type64 - gets symbol type for weak binding
 * @symbol: the symbol to check
 * Return: the symbol type character
 */
char get_weak_type64(Elf64_Sym symbol)
{
	if (symbol.st_shndx == SHN_UNDEF)
		return ('w');
	if (ELF64_ST_TYPE(symbol.st_info) == STT_OBJECT)
		return ('V');
	return ('W');
}

/**
 * get_section_type64 - gets symbol type based on section
 * @symbol: the symbol to check
 * @section_headers: pointer to section headers array
 * Return: the symbol type character
 */
char get_section_type64(Elf64_Sym symbol, Elf64_Shdr *section_headers)
{
	Elf64_Shdr sym_section;

	if (ELF64_ST_BIND(symbol.st_info) == STB_GNU_UNIQUE)
		return ('u');
	sym_section = section_headers[symbol.st_shndx];
	if (sym_section.sh_type == SHT_NOBITS &&
		sym_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ('B');
	if (sym_section.sh_type == SHT_DYNAMIC)
		return ('D');
	if (sym_section.sh_type == SHT_INIT_ARRAY ||
		sym_section.sh_type == SHT_FINI_ARRAY)
		return ('d');
	if (sym_section.sh_type == SHT_NOTE)
		return ('r');
	if (sym_section.sh_type == SHT_PROGBITS)
	{
		if (sym_section.sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
			return ('T');
		if (sym_section.sh_flags == SHF_ALLOC)
			return ('R');
		if (sym_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
			return ('D');
	}
	return ('t');
}

/**
 * get_symbol_type64 - determines the symbol type character
 * @symbol: the symbol to check
 * @section_headers: pointer to section headers array
 * Return: the symbol type character
 */
char get_symbol_type64(Elf64_Sym symbol, Elf64_Shdr *section_headers)
{
	if (ELF64_ST_BIND(symbol.st_info) == STB_WEAK)
		return (get_weak_type64(symbol));
	if (symbol.st_shndx == SHN_UNDEF)
		return ('U');
	if (symbol.st_shndx == SHN_ABS)
		return ('A');
	if (symbol.st_shndx == SHN_COMMON)
		return ('C');
	if (symbol.st_shndx < SHN_LORESERVE)
		return (get_section_type64(symbol, section_headers));
	return ('?');
}

/**
 * read_elf_header64 - reads and validates the ELF header
 * @file: pointer to the open file
 * @file_path: path to the file
 * @elf_header: pointer to store the header
 * Return: 1 on success, 0 on failure
 */
int read_elf_header64(FILE *file, char *file_path, Elf64_Ehdr *elf_header)
{
	fread(elf_header, sizeof(Elf64_Ehdr), 1, file);
	if (elf_header->e_ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "./hnm: %s: unsupported ELF file format\n",
			file_path);
		return (0);
	}
	if (elf_header->e_ident[EI_DATA] != ELFDATA2LSB &&
		elf_header->e_ident[EI_DATA] != ELFDATA2MSB)
	{
		fprintf(stderr, "./hnm: %s: unsupported ELF endianness\n",
			file_path);
		return (0);
	}
	return (1);
}

/**
 * find_symbol_table64 - finds the symbol table index
 * @section_headers: pointer to section headers array
 * @shnum: number of section headers
 * Return: index of symbol table or -1 if not found
 */
int find_symbol_table64(Elf64_Shdr *section_headers, int shnum)
{
	int i;

	for (i = 0; i < shnum; i++)
	{
		if (section_headers[i].sh_type == SHT_SYMTAB)
			return (i);
	}
	return (-1);
}
