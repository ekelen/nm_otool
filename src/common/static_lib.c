#include <nm_otool.h>

static int		get_size(char *name)
{
	int		x;
	char	*word;

	word = ft_strchr(name, '/') + 1;
	x = ft_atoi(word);
	return (x);
}

static char	*get_ar_name(char *name)
{
	int		len;

	len = ft_strlen(ARFMAG);
	return (ft_strstr(name, ARFMAG) + len);
}

static t_ofile *init_ofile(t_file *file, void *ptr, t_ar_hdr *header)
{
    t_ofile *new;
    
    int result_init_ofile;
    if (!(new = (t_ofile *)malloc(sizeof(t_ofile))))
        return NULL;
    
    new->member_ar_hdr = header;
    new->member_name_size = (size_t)get_size(new->member_ar_hdr->ar_name);
    new->member_size = (uint32_t)ft_atoi(new->member_ar_hdr->ar_size);
    new->member_name = (char *)get_ar_name(new->member_ar_hdr->ar_name);
    new->member_addr = (void*)ptr 
        + sizeof(struct ar_hdr) 
        + new->member_name_size;
    return new;
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

void	*get_ar_string_table(t_file *file, void *ranlib_arr, size_t nranlibs)
{
	void *string_table;

	string_table = ranlib_arr
								+ (nranlibs)
								+ sizeof(uint32_t);

	if (!(ptr_check_msg(file->end, string_table, 0, "string table")))
        return NULL;
	return (string_table);
}

int handle_archive(t_file *file)
{
    // ft_printf("Handling archive.\n");
    struct ar_hdr	*header;
    void            *ptr;
    size_t          ar_size;
    // int size;
    // struct ranlib *ran;
    // void *strtab;
    t_ofile *ofile;

    int i = -1;

    if (!(header = (t_ar_hdr *)ptr_check_msg(file->end, (void*)file->data + file->offset, sizeof(t_ar_hdr), "archive header")))
        return EXIT_FAILURE;
   
    ar_size = (size_t)ft_atoi(header->ar_size);
    if (!(ptr = ptr_check_msg(file->end, (void*)header + ar_size + sizeof(struct ar_hdr), 0, "first ptr")))
        return EXIT_FAILURE;

    if (!(header = (struct ar_hdr *)ptr_check_msg(file->end, ptr, sizeof(struct ar_hdr), "second header")))
        return EXIT_FAILURE;
    ptr = header;

    i = -1;
    while (ptr)
	{
        if (ptr == file->end)
        {
            // print_ofiles(file, file->ofile);
            return EXIT_SUCCESS;
        }
        // ft_printf("======================= 1\n");
		if (!(header = (t_ar_hdr *)ptr_check_msg(file->end, ptr, sizeof(struct ar_hdr), "ar_hdr")))
        {
            // ft_printf("ptr  ::  %p\n", ptr);
            // ft_printf("file->end  ::  %p\n", file->end);
        	return EXIT_FAILURE;
        }
            // ft_printf("======================= 2\n");
		if (ft_atoi(header->ar_size) <= 0)
			return (EXIT_FAILURE);
            // ft_printf("======================= 3\n");
        if (add_ofile(file, ptr, header) == EXIT_FAILURE)
            return EXIT_FAILURE;

        // print_ofiles(file, file->ofile);
        
        // ft_printf("======================= 4\n");
        
        if (!(ptr = ptr_check_msg(file->end, ptr + ft_atoi(header->ar_size) + sizeof(struct ar_hdr), 0, "new ptr")))
            return EXIT_FAILURE;
            
        // ft_printf("======================= 5\n");
	}
    return EXIT_SUCCESS;
}

	// n_objs = *((int *)((void*)header + sizeof(*header) + get_size(header->ar_name)));
    // ran = (struct ranlib*)((int *)((void*)header + sizeof(*header) + get_size(header->ar_name)) + 1);

    // while(++i < (n_objs / sizeof(struct ranlib)))
    // {
    //     ft_printf("i  ::  %d\n", i);
    //     ft_printf("ran[i].ran_off  ::  %zu\n", ran[i].ran_off);
    // }