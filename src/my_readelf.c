#include <elf.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "struct_opt.h"

static int is_elf(void *e_map)
{
	Elf64_Ehdr *ehdr = e_map;
	int magic_nb_valid = 0;
	magic_nb_valid += !(ehdr->e_ident[0] == ELFMAG0);
	magic_nb_valid += !(ehdr->e_ident[1] == ELFMAG1);
	magic_nb_valid += !(ehdr->e_ident[2] == ELFMAG2);
	magic_nb_valid += !(ehdr->e_ident[3] == ELFMAG3);
	return !magic_nb_valid;
}

static int has_opt(int len, char **cmd)
{
	for (int i = 0; i < len; i++)
		if (cmd[i][0] == '-')
			return 1;
	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 3 || !has_opt(argc, argv))
	{
		display_opt();
		return 1;
	}
	int e_fd = open(argv[2], O_RDONLY);
	if (e_fd == -1)
		err(1, "main(): open(): ");
	struct stat info;
	if (fstat(e_fd, &info) == -1)
		err(1, "main(): fstat(): ");
	if (!info.st_size)
		err(1, "file size = 0");
	void *e_map = mmap(NULL, info.st_size, PROT_READ, MAP_PRIVATE, e_fd, 0);
	if (close(e_fd) < 0)
		err(1, "main(): close(): ");
	if (e_map == MAP_FAILED)
		err(1, "main(): mmap(): ");
	if (!is_elf(e_map))
		err(1, "%s: Failed to read file's magic number", argv[2]);
	run_opt(argc, argv, e_map);
	if (munmap(e_map, info.st_size) < 0)
		err(1, "main(): munmap(): %s", strerror(errno));
	return 0;
}
