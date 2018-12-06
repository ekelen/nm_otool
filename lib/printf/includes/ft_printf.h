/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/20 21:27:40 by ekelen            #+#    #+#             */
/*   Updated: 2018/12/05 15:16:19 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <wchar.h>
# include <locale.h>

# define FLAGS		"-#0+ "
# define TF			"sSpdDioOuUxXcC%"
# define LN			tg->len_mod
# define MIN_WIDTH	tg->min_width
# define LEFT		tg->left_align

typedef struct	s_arg
{
	int		lead_zeroes;
	int		left_align;
	int		prefix;
	int		lead_space;
	int		lead_plus;
	int		is_negative;

	size_t	min_width;
	size_t	precision;
	int		prec_flag;
	char	len_mod[3];
	char	form;
	char	num_prefix;
	int		base;
	size_t	len;
}				t_arg;

int				ft_printf(const char *format, ...);
void			get_va_arg(t_arg *tg, va_list ap);
void			zero_tg(t_arg *tg);
void			decide_flags(t_arg *tg, char **s);
void			decide_width(t_arg *tg, char **s);
void			decide_precision(t_arg *tg, char **s);
void			decide_len_mod(t_arg *tg, char **s);
void			write_padding(char padding, t_arg *tg, size_t num_spaces);
void			write_padding_static(char padding, size_t num_spaces);
void			handle_percent(char percent_char, t_arg *tg);
void			handle_str(char *arg_str, t_arg *tg);
void			handle_char(int c, t_arg *tg);
void			handle_int(intmax_t arg_int, t_arg *tg);
char			*ft_itoa_abs(intmax_t n, t_arg *tg);
void			get_int_padding(t_arg *tg, char *arg_str, int sign);
void			write_int(t_arg *tg, char **arg_str);
void			handle_uint(uintmax_t arg_uint, t_arg *tg);
char			*ft_f_itoa_base(uintmax_t nbr, int base, t_arg *tg);
void			get_uint_lenmod(t_arg *tg);
void			format_hex_zero(t_arg *tg, char **arg_str);
void			format_uint_zero(t_arg *tg, char **arg_str);
void			handle_wchar(int c, t_arg *tg);
void			locale_check(int c, t_arg *tg);
char			**ft_itobyte(unsigned int b, char wchar_len);
void			handle_wstr_helper(wchar_t *arg_wstr, t_arg *tg);
int				ft_isdigit(int c);
void			ft_memdel(void **ap);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char const *s, int fd);
char			*ft_strchr(const char *s, int c);
void			ft_strclr(char *s);
char			*ft_strcpy(char *dst, const char *src);
void			ft_strdel(char **as);
size_t			ft_strlen(const char *s);
void			ftek_strnclr(char *s, size_t len);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strdup(const char *s1);
char			**ft_strsplit(char const *s, char c);
char			*ft_strncpy(char *dst, const char *src, size_t len);
void			*ft_memset(void *b, int c, size_t len);
char			*ft_strnew(size_t size);
int				ft_f_atoi_base(char *str, int base);

#endif
