#include <nm_otool.h>
#include <assert.h>

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

// store ar_hdr info along with each mach-o in archive
int add_ofile(t_file *file, void *ptr, const void *header)
{
    t_ofile o;
    t_mach_o *m;

    o.hdr = (const t_ar_hdr *)header;
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

// Look through an archive header
static int check_archive_ptrs(t_file *file, void **ptr, t_ar_hdr **hdr, size_t *ar_size)
{
    if (!(*hdr = (t_ar_hdr *)ptr_check_msg(file->end, *ptr, AR_HDR_SIZE, "ar_hdr")))
        return EXIT_FAILURE;
    if (ft_atoi((*hdr)->ar_size) <= 0)
        return (EXIT_FAILURE);
    if (add_ofile(file, *ptr, *ptr) == EXIT_FAILURE)
        return EXIT_FAILURE;
    *ar_size = ft_atoi((*hdr)->ar_size);
    return EXIT_SUCCESS;
}

int handle_archive(t_file *file)
{
    struct ar_hdr	*header;
    void            *ptr;
    size_t          ar_size;

    if (!(header = (t_ar_hdr *)ptr_check_msg(file->end, file->data + file->offset, sizeof(t_ar_hdr), "archive header")))
        return (EXIT_FAILURE);
    ar_size = (size_t)ft_atoi(header->ar_size);
    if (!(ptr = ptr_check_msg(file->end, (void*)header + ar_size + AR_HDR_SIZE, 0, "first ptr")))
        return (EXIT_FAILURE);
    while (ptr < file->end) // TODO: Seems rickety
	{
        if (check_archive_ptrs(file, &ptr, &header, &ar_size) == EXIT_FAILURE)
            return (EXIT_FAILURE);
        if (!(ptr = ptr_check_msg(file->end, ptr + ar_size + AR_HDR_SIZE, 0, "new ptr")))
            return (EXIT_FAILURE);
	}
    return (EXIT_SUCCESS);
}