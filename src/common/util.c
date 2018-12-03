#include <nm_otool.h>


const void *ptr_check(void *addr_max, const void *req, size_t req_length) 
{
    if ((uint8_t *) req + req_length > (uint8_t *) addr_max) {
        return NULL;
    }
    return req;
}

const void *ptr_check_msg(void *addr_max, const void *req, size_t req_length, const char *msg) 
{
    if ((uint8_t *) req + req_length > (uint8_t *) addr_max) {
        ft_putstr_fd("Not enough room for ", 2);
        ft_putendl_fd(msg, 2);
        return NULL;
    }
    return req;
}

uint32_t swap32 (uint32_t x) {
    return ((uint32_t)((((uint32_t)(x) & 0xff000000) >> 24) | \
                (((uint32_t)(x) & 0x00ff0000) >>  8) | \
                (((uint32_t)(x) & 0x0000ff00) <<  8) | \
                (((uint32_t)(x) & 0x000000ff) << 24)));
}

uint64_t	swap64(uint64_t x)
{
	x = ((x & 0x00000000000000FF) << 56 |
				(x & 0x000000000000FF00) << 40 |
				(x & 0x0000000000FF0000) << 24 |
				(x & 0x00000000FF000000) << 8 |
				(x & 0x000000FF00000000) >> 8 |
				(x & 0x0000FF0000000000) >> 24 |
				(x & 0x00FF000000000000) >> 40 |
				(x & 0xFF00000000000000) >> 56);
	return (x);
}

uint64_t	nswap64(uint64_t x)
{
	return (x);
}

uint32_t nswap32(uint32_t x) {
    return x;
}