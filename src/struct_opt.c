#include <stdio.h>
#include <string.h>

#include "struct_opt.h"

struct opt *get_opt(void)
{
	static struct opt opt[] =
	{
		{"-h", "--file-header", "Display the ELF file header", print_header}
	};
	return opt;
}

void display_opt(void)
{
	struct opt *opt = get_opt();
	puts("Usage: readelf <option(s)> elf-file(s)");
	puts(" Display information about the contents of ELF format files");
	puts(" Options are:");
	if (!opt)
		return;
	for (size_t i = 0; i < NB_OPT; i++)
		printf("  %s %s\t%s\n", opt[i].short_opt, opt[i].long_opt, opt[i].description);

}

int run_opt(int len, char **cmd, void *e_map)
{
	struct opt *opt = get_opt();
	if (!opt)
		return 1;
	for (int i = 1; i < len; i++)
	{
		for (int j = 0; cmd[i][0] == '-' && j < NB_OPT; j++)
			if (!strcmp(cmd[i], opt[j].short_opt)
					|| !strcmp(cmd[i], opt[j].short_opt))
				opt[j].func(e_map);
	}
	return 0;
}
