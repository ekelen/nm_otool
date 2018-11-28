# ----------------------------------------------------------------------------
# COLORS |
# ----------------------------------------------------------------------------
 RESET=\033[0m
 DARK=\033[132m
 RED=\033[31m
 GREEN=\033[32m
 YELLOW=\033[33m
 BLUE=\033[34m
 MAGENTA=\033[35m
 CYAN=\033[36m
 WHITE=\033[37m
 BOLDBLACK=\033[1m\033[30m
 BOLDRED=\033[1m\033[31m
 BOLDWHITE=\033[1m\033[37m

.PHONY: all, clean, fclean, re

# ----------------------------------------------------------------------------
# VARIABLES |
# ----------------------------------------------------------------------------
NAME = libftprintf.a

CC = gcc
CFLAGS += -Wall -Werror -Wextra -I.

SRC_PATH = ./srcs/

# CORE
SRC_NAME += ft_printf.c
SRC_NAME += raw_parse.c 
SRC_NAME += get_va_arg.c

# HANDLE ARG TYPES
SRC_NAME += handle_string.c
SRC_NAME += handle_char.c
SRC_NAME += handle_wchar.c
SRC_NAME += handle_wstr.c
SRC_NAME += handle_int.c
SRC_NAME += handle_uint.c
SRC_NAME += handle_percent.c

# ITOA AND ATOI
SRC_NAME += ft_itoa_abs.c
SRC_NAME += ft_itoa_base.c
SRC_NAME += ft_atoi_base.c
SRC_NAME += ft_itobyte.c

# MISC FORMATTING
SRC_NAME += write_padding.c
SRC_NAME += uint_helper.c

# LIBFT STRAGGLERS
SRC_NAME += ft_isdigit.c
SRC_NAME += ft_memdel.c
SRC_NAME += ft_putchar_fd.c
SRC_NAME += ft_putstr_fd.c
SRC_NAME += ft_strchr.c
SRC_NAME += ft_strclr.c
SRC_NAME += ft_strcmp.c
SRC_NAME += ft_strcpy.c
SRC_NAME += ft_strdel.c
SRC_NAME += ft_strdup.c
SRC_NAME += ft_strlen.c
SRC_NAME += ftek_strnclr.c
SRC_NAME += ft_strncpy.c
SRC_NAME += ft_strsplit.c
SRC_NAME += ft_strnew.c
SRC_NAME += ft_memset.c


SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))

OBJ_PATH = ./obj/
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))


INCLUDES = ./includes/
INC_CC = $(foreach DIR,$(INCLUDES),-I$(DIR) )
CFLAGS += $(INC_CC)

# ----------------------------------------------------------------------------
# MISC |
# ----------------------------------------------------------------------------
$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@echo -n .
	@$(CC) $(CFLAGS) -o $@ -c $<

# ----------------------------------------------------------------------------
# RULES |
# ----------------------------------------------------------------------------
all: $(NAME)

bin:
	gcc main.c $(INC_CC) -L./ $(NAME) -lftprintf

$(NAME): $(OBJ)
	@$(AR) rcs $(NAME) $^
	@echo ""
	@echo "[$(GREEN)$(NAME) compiled.$(RESET)]"
	
clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all
