#include <nm_otool.h>

// Add result to ar_hdr pointer to get position of mach-O hdr
static int		get_size(const char *name)
{
	int		x;
	char	*word;

	word = ft_strchr(name, '/') + 1;
	x = ft_atoi(word);
	return (x);
}

// Get name of archive object file by looking after the position of ARFMAG
static char	    *get_ar_name(const char *name)
{
	int		len;

	len = ft_strlen(ARFMAG);
	return (ft_strstr(name, ARFMAG) + len);
}

int add_ofile(t_file *file, void *ptr, const t_ar_hdr *header)
{
    t_ofile o;
    t_mach_o *m;

    o.hdr = header;
    o.name_size = (size_t)get_size(o.hdr->ar_name);
    o.size = (uint32_t)ft_atoi(o.hdr->ar_size);
    o.name = (const char *)get_ar_name(o.hdr->ar_name);
    o.addr = ptr 
        + sizeof(t_ar_hdr) 
        + o.name_size;
    if (!(m = init_mach_o(file, o.addr, o.size - o.name_size)))
        return EXIT_FAILURE;
    m->ofile = o;
    return(add_mach(&(file->mach), m));
}

int handle_archive(t_file *file)
{
    struct ar_hdr	*header;
    void            *ptr;
    size_t          ar_size;
    int             i;

    if (!(header = (t_ar_hdr *)ptr_check_msg(file->end, (void*)file->data + file->offset, sizeof(t_ar_hdr), "archive header")))
        return EXIT_FAILURE;
    ar_size = (size_t)ft_atoi(header->ar_size);
    if (!(ptr = ptr_check_msg(file->end, (void*)header + ar_size + sizeof(struct ar_hdr), 0, "first ptr")))
        return EXIT_FAILURE;
    if (!(header = (struct ar_hdr *)ptr_check_msg(file->end, ptr, sizeof(struct ar_hdr), "second header")))
        return EXIT_FAILURE;
    ptr = header;
    i = -1;
    while (ptr) // TODO: Seems rickety
	{
        if (ptr == file->end) // TODO: Really? Always?
        {
            return EXIT_SUCCESS;
        }
		if (!(header = (t_ar_hdr *)ptr_check_msg(file->end, ptr, sizeof(struct ar_hdr), "ar_hdr")))
        {
        	return EXIT_FAILURE;
        }
		if (ft_atoi(header->ar_size) <= 0)
			return (EXIT_FAILURE);
        if (add_ofile(file, ptr, header) == EXIT_FAILURE)
            return EXIT_FAILURE;
        if (!(ptr = ptr_check_msg(file->end, ptr + ft_atoi(header->ar_size) + sizeof(struct ar_hdr), 0, "new ptr")))
            return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}