#ifndef HNM_H
#define HNM_H

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <ctype.h>

/* hnm-32.c */
void print_symbol_table32(Elf32_Shdr *section_header, Elf32_Sym *symbol_table,
			  char *string_table, Elf32_Shdr *section_headers);
int load_tables32(FILE *file, char *file_path, Elf32_Shdr *section_headers,
		int shnum, Elf32_Sym **sym_table, char **str_table,
		Elf32_Shdr *sym_header);
void process_elf_file32(char *file_path);

/* hnm-32-helpers.c */
char get_weak_type32(Elf32_Sym symbol);
char get_section_type32(Elf32_Sym symbol, Elf32_Shdr *section_headers);
char get_symbol_type32(Elf32_Sym symbol, Elf32_Shdr *section_headers);
int read_elf_header32(FILE *file, char *file_path, Elf32_Ehdr *elf_header);
int find_symbol_table32(Elf32_Shdr *section_headers, int shnum);

/* hnm-64.c */
void print_symbol_table64(Elf64_Shdr *section_header, Elf64_Sym *symbol_table,
			  char *string_table, Elf64_Shdr *section_headers);
int load_tables64(FILE *file, char *file_path, Elf64_Shdr *section_headers,
		int shnum, Elf64_Sym **sym_table, char **str_table,
		Elf64_Shdr *sym_header);
void process_elf_file64(char *file_path);

/* hnm-64-helpers.c */
char get_weak_type64(Elf64_Sym symbol);
char get_section_type64(Elf64_Sym symbol, Elf64_Shdr *section_headers);
char get_symbol_type64(Elf64_Sym symbol, Elf64_Shdr *section_headers);
int read_elf_header64(FILE *file, char *file_path, Elf64_Ehdr *elf_header);
int find_symbol_table64(Elf64_Shdr *section_headers, int shnum);

#endif /* HNM_H */
