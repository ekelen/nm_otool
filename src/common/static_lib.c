#include <nm_otool.h>

static int		get_size(char *name)
{
	int		x;
	char	*word;

	word = ft_strchr(name, '/') + 1;
	x = ft_atoi(word);
	return (x);
}

static char	    *get_ar_name(const char *name)
{
	int		len;

	len = ft_strlen(ARFMAG);
	return (ft_strstr(name, ARFMAG) + len);
}

// TODO: Cleanup/rmv heap alloc
// Also I don't like the name of this struct
static t_ofile *init_ofile(t_file *file, void *ptr, const t_ar_hdr *header)
{
    t_ofile *o;
    
    int result_init_ofile;
    if (!(o = (t_ofile *)malloc(sizeof(t_ofile))))
        return NULL;
    
    o->member_ar_hdr = header;
    o->member_name_size = (size_t)get_size(o->member_ar_hdr->ar_name);
    o->member_size = (uint32_t)ft_atoi(o->member_ar_hdr->ar_size);
    o->member_name = (char *)get_ar_name(o->member_ar_hdr->ar_name);
    o->member_addr = (void*)ptr 
        + sizeof(struct ar_hdr) 
        + o->member_name_size;
    return o;
}

// TODO: I don't like the name of this struct
static t_ofile init_o(t_file *file, void *ptr, t_ar_hdr *header)
{
    t_ofile o;
    return o;
    // t_ofile *o;
    
    // int result_init_ofile;
    // if (!(o = (t_ofile *)malloc(sizeof(t_ofile))))
    //     return NULL;
    
    // o->member_ar_hdr = header;
    // o->member_name_size = (size_t)get_size(o->member_ar_hdr->ar_name);
    // o->member_size = (uint32_t)ft_atoi(o->member_ar_hdr->ar_size);
    // o->member_name = (const char *)get_ar_name(o->member_ar_hdr->ar_name);
    // o->member_addr = (void*)ptr 
    //     + sizeof(struct ar_hdr) 
    //     + o->member_name_size;
    // return o;
}

int add_ofile(t_file *file, void *ptr, t_ar_hdr *header)
{
    t_ofile *new;
    t_mach_o *m;
    int result;

    if (!(new = init_ofile(file, ptr, header)))
        return EXIT_FAILURE;
    if (!(m = init_mach_o(file, new->member_addr, new->member_size - new->member_name_size)))
        return NULL;
    m->ofile = new;
    result = add_mach(&(file->mach), m);
    return result;
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