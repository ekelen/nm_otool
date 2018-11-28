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
 
NAME_NM = ft_nm
NAME_OTOOL = ft_otool

PATH_SRC = src
PATH_INC = inc
PATH_OBJ = obj

PATH_NM = nm
PATH_OTOOL = otool
PATH_COMMON = common

PATH_TEST = test

PATH_LIB = lib
PATH_LIBFT = $(PATH_LIB)/libft
PATH_PRINTF = $(PATH_LIB)/printf

INC = -I$(PATH_INC) -I$(PATH_LIBFT)/inc -I$(PATH_PRINTF)/includes

CC = gcc
CFLAGS = -Werror -Wextra -w -g $(INC)

SRC_COMMON += error.c

SRC_COMMON += fat.c
SRC_COMMON += mach.c
SRC_NM += main.c
SRC_COMMON += init.c
SRC_COMMON += util.c
SRC_COMMON += section.c
SRC_COMMON += static_lib.c
SRC_COMMON += symbol.c
SRC_COMMON += parse_commands.c
SRC_COMMON += parse_symtab.c
SRC_COMMON += print.c

SRC_OTOOL += main.c

SRCS_COMMON = $(addprefix $(PATH_SRC)/$(PATH_COMMON)/,$(SRC_COMMON))
OBJ_COMMON = $(notdir $(SRCS_COMMON:.c=.o))
OBJS_COMMON = $(addprefix $(PATH_OBJ)/$(PATH_COMMON)/,$(OBJ_COMMON))

SRCS_NM = $(addprefix $(PATH_SRC)/$(PATH_NM)/,$(SRC_NM))
OBJ_NM = $(notdir $(SRCS_NM:.c=.o))
OBJS_NM = $(addprefix $(PATH_OBJ)/$(PATH_NM)/,$(OBJ_NM))

SRCS_OTOOL = $(addprefix $(PATH_SRC)/$(PATH_OTOOL)/,$(SRC_OTOOL))
OBJ_OTOOL = $(notdir $(SRCS_OTOOL:.c=.o))
OBJS_OTOOL = $(addprefix $(PATH_OBJ)/$(PATH_OTOOL)/,$(OBJ_OTOOL))

LIBS = -L$(PATH_LIBFT)/ -lft -L$(PATH_PRINTF)/ -lftprintf

all: $(OBJS_COMMON) $(OBJS_NM) $(OBJS_OTOOL)
	@$(CC) -o $(NAME_NM) $(OBJS_COMMON) $(OBJS_NM) $(LIBS)
	@$(CC) -o $(NAME_OTOOL) $(OBJS_COMMON) $(OBJS_OTOOL) $(LIBS)

libs:
	@make -C $(PATH_LIBFT) re
	@make -C $(PATH_LIBFT) clean
	@make -C $(PATH_PRINTF) re
	@make -C $(PATH_PRINTF) clean

$(OBJS_COMMON): $(PATH_OBJ)/$(PATH_COMMON)/%.o: $(PATH_SRC)/$(PATH_COMMON)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_NM): $(PATH_OBJ)/$(PATH_NM)/%.o: $(PATH_SRC)/$(PATH_NM)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_OTOOL): $(PATH_OBJ)/$(PATH_OTOOL)/%.o: $(PATH_SRC)/$(PATH_OTOOL)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

test: re
	@echo -e "${MAGENTA}"
	# ./ft_nm test/unit_test_files/lib_stat/libft_static.a
	./ft_nm test/unit_test_files/lib_stat/libft_static.a > libft_static.mine.txt
	nm test/unit_test_files/lib_stat/libft_static.a > libft_static.actual.txt
	diff libft_static.mine.txt libft_static.actual.txt > libft_static.diff.txt
	@echo -e "$(RESET)"
	# nm test/unit_test_files/fat/audiodevice
	# nm test/unit_test_files/fat/audiodevice > fat_expected.txt
	# diff fat_mine.txt fat_expected.txt > fat_diff.txt

clean:
	@rm -f $(OBJS_COMMON)
	@rm -f $(OBJS_NM)

fclean: clean
	@rm -f $(NAME_NM)
	@echo "${MAGENTA}cleaned.${RESET}" >&2


fcleanlibs:
	@make -C $(PATH_LIBFT) fclean
	@make -C $(PATH_PRINTF) fclean

re: fclean all