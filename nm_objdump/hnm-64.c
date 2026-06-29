#include "hnm.h"

/**
 * print_symbol_table64 - prints the symbol table for a 64-bit ELF file
 * @section_header: pointer to the section header of the symbol table
 * @symbol_table: pointer to the beginning of the symbol table
 * @string_table: pointer to the beginning of the string table
 * @section_headers: pointer to the array of section headers
 * Return: nothing (void)
 */
void print_symbol_table64(Elf64_Shdr *section_header, Elf64_Sym *symbol_table,
			  char *string_table, Elf64_Shdr *section_headers)
{
	int i;
	int symbol_count;
	char *symbol_name, symbol_type;
	Elf64_Sym symbol;

	symbol_count = section_header->sh_size / sizeof(Elf64_Sym);
	for (i = 0; i < symbol_count; i++)
	{
		symbol = symbol_table[i];
		symbol_name = string_table + symbol.st_name;
		if (symbol.st_name == 0 ||
			ELF64_ST_TYPE(symbol.st_info) == STT_FILE)
			continue;
		symbol_type = get_symbol_type64(symbol, section_headers);
		if (ELF64_ST_BIND(symbol.st_info) == STB_LOCAL)
			symbol_type = tolower(symbol_type);
		if (symbol_type != 'U' && symbol_type != 'w')
			printf("%016lx %c %s\n",
				symbol.st_value, symbol_type, symbol_name);
		else
			printf("                 %c %s\n",
				symbol_type, symbol_name);
	}
}

/**
 * load_tables64 - loads symbol and string tables from ELF file
 * @file: pointer to the open file
 * @file_path: path to the file
 * @section_headers: pointer to section headers array
 * @shnum: number of section headers
 * @sym_table: address to store allocated symbol table
 * @str_table: address to store allocated string table
 * @sym_header: pointer to store symbol table section header
 * Return: 1 on success, 0 on failure
 */
int load_tables64(FILE *file, char *file_path, Elf64_Shdr *section_headers,
		int shnum, Elf64_Sym **sym_table, char **str_table,
		Elf64_Shdr *sym_header)
{
	int idx;
	Elf64_Shdr str_header;

	idx = find_symbol_table64(section_headers, shnum);
	if (idx == -1)
	{
		fprintf(stderr, "./hnm: %s: no symbols\n", file_path);
		return (0);
	}
	*sym_header = section_headers[idx];
	*sym_table = malloc(sym_header->sh_size);
	fseek(file, sym_header->sh_offset, SEEK_SET);
	fread(*sym_table, sym_header->sh_size, 1, file);
	str_header = section_headers[sym_header->sh_link];
	*str_table = malloc(str_header.sh_size);
	fseek(file, str_header.sh_offset, SEEK_SET);
	fread(*str_table, str_header.sh_size, 1, file);
	return (1);
}

/**
 * process_elf_file64 - processes a 64-bit ELF file
 * @file_path: path to the ELF file to be processed
 * Return: nothing (void)
 */
void process_elf_file64(char *file_path)
{
	Elf64_Ehdr elf_header;
	Elf64_Shdr *section_headers, sym_header;
	Elf64_Sym *symbol_table;
	char *string_table;
	FILE *file;

	file = fopen(file_path, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "./hnm: %s: failed to open file\n", file_path);
		return;
	}
	if (!read_elf_header64(file, file_path, &elf_header))
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
	fread(section_headers, elf_header.e_shentsize, elf_header.e_shnum,
		file);
	if (!load_tables64(file, file_path, section_headers,
		elf_header.e_shnum, &symbol_table, &string_table, &sym_header))
	{
		free(section_headers);
		fclose(file);
		return;
	}
	print_symbol_table64(&sym_header, symbol_table,
		string_table, section_headers);
	fclose(file);
	free(section_headers);
	free(symbol_table);
	free(string_table);
}
