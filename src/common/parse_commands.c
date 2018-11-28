#include <nm_otool.h>

int parse_seg(t_file *file, t_mach_o *m, const struct load_command *lc)
{
	t_segment_command *seg;
	t_segment_command_64 *seg64;
	uint32_t nsects;

	seg = !m->m64 ? (t_segment_command *)ptr_check_msg(m->end, lc, m->swap32(lc->cmdsize), "segment") : NULL;
	seg64 = m->m64 ? (t_segment_command_64 *)ptr_check_msg(m->end, lc, m->swap32(lc->cmdsize), "segment 64") : NULL;

	if (!seg && !seg64)
		return EXIT_FAILURE;
	nsects = m->m64 ? m->swap32(seg64->nsects) : m->swap32(seg->nsects);
    if (nsects > 0)
        return get_secs(file, m, seg ? (void*)seg : (void*)seg64, nsects);
    return EXIT_SUCCESS;
}
