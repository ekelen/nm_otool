#include <nm_otool.h>
#include <assert.h>

void print_nsecs(t_mach_o *m)
{
	// (void)m;
    ft_printf("TEXT  ::  %zu\n", (m->nsects & TEXT_SECT));
    ft_printf("BSS   ::  %zu\n", (m->nsects & BSS_SECT) >> 16);
    ft_printf("DATA  ::  %zu\n", (m->nsects & DATA_SECT) >> 8);
}

