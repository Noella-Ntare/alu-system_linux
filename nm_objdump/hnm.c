#include "hnm.h"

/**
 * main - entry point for the hnm program
 * reproduces the GNU nm command with nm -p output format
 * @argc: number of command line arguments
 * @argv: array of command line argument strings
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
	int i;
	FILE *file;
	unsigned char e_ident[EI_NIDENT];

	if (argc < 2)
	{
		fprintf(stderr, "Usage: hnm [objfile ...]\n");
		return (1);
	}
	for (i = 1; i < argc; i++)
	{
		file = fopen(argv[i], "rb");
		if (file == NULL)
		{
			fprintf(stderr, "./hnm: %s: No such file\n", argv[i]);
			continue;
		}
		fread(e_ident, 1, EI_NIDENT, file);
		fclose(file);
		if (e_ident[EI_CLASS] == ELFCLASS64)
			process_elf_file64(argv[i]);
		else if (e_ident[EI_CLASS] == ELFCLASS32)
			process_elf_file32(argv[i]);
		else
			fprintf(stderr, "./hnm: %s: unsupported format\n",
				argv[i]);
	}
	return (0);
}
