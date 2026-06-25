#include "hnm.h"

/**
 * get_weak_type - gets symbol type for weak binding
 * @symbol: the symbol to check
 * Return: the symbol type character
 */
static char get_weak_type(Elf32_Sym symbol)
{
	if (symbol.st_shndx == SHN_UNDEF)
		return ('w');
	if (ELF32_ST_TYPE(symbol.st_info) == STT_OBJECT)
		return ('V');
	return ('W');
}

/**
 * get_section_type - gets symbol type based on section
 * @symbol: the symbol to check
 * @section_headers: pointer to section headers array
 * Return: the symbol type character
 */
static char get_section_type(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	Elf32_Shdr sym_section;

	if (ELF32_ST_BIND(symbol.st_info) == STB_GNU_UNIQUE)
		return ('u');

	sym_section = section_headers[symbol.st_shndx];

	if (sym_section.sh_type == SHT_NOBITS &&
		sym_section.sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ('B');

	if (sym_section.sh_type == SHT_DYNAMIC)
		return ('D');

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
 * get_symbol_type - determines the symbol type character
 * @symbol: the symbol to check
 * @section_headers: pointer to section headers array
 * Return: the symbol type character
 */
static char get_symbol_type(Elf32_Sym symbol, Elf32_Shdr *section_headers)
{
	if (ELF32_ST_BIND(symbol.st_info) == STB_WEAK)
		return (get_weak_type(symbol));
	if (symbol.st_shndx == SHN_UNDEF)
		return ('U');
	if (symbol.st_shndx == SHN_ABS)
		return ('A');
	if (symbol.st_shndx == SHN_COMMON)
		return ('C');
	if (symbol.st_shndx < SHN_LORESERVE)
		return (get_section_type(symbol, section_headers));
	return ('?');
}

/**
 * print_symbol_table32 - prints the symbol table for a 32-bit ELF file
 * @section_header: pointer to the section header of the symbol table
 * @symbol_table: pointer to the beginning of the symbol table
 * @string_table: pointer to the beginning of the string table
 * @section_headers: pointer to the array of section headers
 * Return: nothing (void)
 */
void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
			  char *string_table, Elf32_Shdr *section_headers)
{
	int i;
	int symbol_count = section_header->sh_size / sizeof(Elf32_Sym);
	char *symbol_name, symbol_type;

	for (i = 0; i < symbol_count; i++)
	{
		Elf32_Sym symbol = symbol_table[i];

		symbol_name = string_table + symbol.st_name;
		if (symbol.st_name == 0 ||
			ELF32_ST_TYPE(symbol.st_info) == STT_FILE)
			continue;

		symbol_type = get_symbol_type(symbol, section_headers);
		if (ELF32_ST_BIND(symbol.st_info) == STB_LOCAL)
			symbol_type = tolower(symbol_type);

		if (symbol_type != 'U' && symbol_type != 'w')
			printf("%08x %c %s\n",
				symbol.st_value, symbol_type, symbol_name);
		else
			printf("         %c %s\n", symbol_type, symbol_name);
	}
}

/**
 * read_elf_header32 - reads and validates the ELF header
 * @file: pointer to the open file
 * @file_path: path to the file
 * @elf_header: pointer to store the header
 * Return: 1 on success, 0 on failure
 */
static int read_elf_header32(FILE *file, char *file_path,
			     Elf32_Ehdr *elf_header)
{
	fread(elf_header, sizeof(Elf32_Ehdr), 1, file);

	if (elf_header->e_ident[EI_CLASS] != ELFCLASS32 &&
		elf_header->e_ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "./hnm: %s: unsupported ELF file format\n",
			file_path);
		return (0);
	}
	if (elf_header->e_ident[EI_DATA] != ELFDATA2LSB &&
		elf_header->e_ident[EI_DATA] != ELFDATA2MSB)
	{
		fprintf(stderr, "./hnm: %s: unsupported ELF file endianness\n",
			file_path);
		return (0);
	}
	return (1);
}

/**
 * find_symbol_table32 - finds the symbol table index
 * @section_headers: pointer to section headers array
 * @shnum: number of section headers
 * Return: index of symbol table or -1 if not found
 */
static int find_symbol_table32(Elf32_Shdr *section_headers, int shnum)
{
	int i;

	for (i = 0; i < shnum; i++)
	{
		if (section_headers[i].sh_type == SHT_SYMTAB)
			return (i);
	}
	return (-1);
}

/**
 * process_elf_file32 - processes a 32-bit ELF file
 * @file_path: path to the ELF file to be processed
 * Return: nothing (void)
 */
void process_elf_file32(char *file_path)
{
	int symbol_table_index;
	int string_table_index;
	Elf32_Ehdr elf_header;
	Elf32_Shdr *section_headers;
	Elf32_Shdr symbol_table_header, string_table_header;
	Elf32_Sym *symbol_table;
	char *string_table;
	FILE *file = fopen(file_path, "rb");

	if (file == NULL)
	{
		fprintf(stderr, "./hnm: %s: failed to open file\n", file_path);
		return;
	}
	if (!read_elf_header32(file, file_path, &elf_header))
	{
		fclose(file);
		return;
	}
	section_headers = malloc(elf_header.e_shentsize * elf_header.e_shnum);
	if (section_headers == NULL)
	{
		fprintf(stderr, "./hnm: %s: memory allocation error\n",
			file_path);
		fclose(file);
		return;
	}
	fseek(file, elf_header.e_shoff, SEEK_SET);
	fread(section_headers, elf_header.e_shentsize,
		elf_header.e_shnum, file);

	symbol_table_index = find_symbol_table32(section_headers,
		elf_header.e_shnum);
	if (symbol_table_index == -1)
	{
		fprintf(stderr, "./hnm: %s: no symbols\n", file_path);
		fclose(file);
		free(section_headers);
		return;
	}
	symbol_table_header = section_headers[symbol_table_index];
	symbol_table = malloc(symbol_table_header.sh_size);
	fseek(file, symbol_table_header.sh_offset, SEEK_SET);
	fread(symbol_table, symbol_table_header.sh_size, 1, file);

	string_table_index = symbol_table_header.sh_link;
	string_table_header = section_headers[string_table_index];
	string_table = malloc(string_table_header.sh_size);
	fseek(file, string_table_header.sh_offset, SEEK_SET);
	fread(string_table, string_table_header.sh_size, 1, file);

	print_symbol_table32(&symbol_table_header, symbol_table,
		string_table, section_headers);

	fclose(file);
	free(section_headers);
	free(symbol_table);
	free(string_table);
}
