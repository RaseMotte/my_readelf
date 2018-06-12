#ifndef _STRUCT_OPT_H_
# define _STRUCT_OPT_H_

# define NB_OPT 1

struct opt{
	char *short_opt;
	char *long_opt;
	char *description;
	int (*func)(void*);
};


/**
 * file : print_header.c
 **/

int print_header(void *e_map);

/**
 * file : struct_opt.c
 **/

struct opt *get_opt(void);
void display_opt(void);
int run_opt(int len, char **cmd, void *e_map);

#endif
