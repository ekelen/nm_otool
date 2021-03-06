RESET=\033[0m
GREEN=\033[32m
BLUE=\033[34m
MAGENTA=\033[35m
CYAN=\033[36m

NAME_NM = ft_nm
NAME_OTOOL = ft_otool

PATH_SRC = src
PATH_INC = inc
PATH_OBJ = obj

PATH_NM = nm
PATH_OTOOL = otool
PATH_COMMON = common

PATH_LIB = lib
PATH_LIBFT = $(PATH_LIB)/libft
PATH_PRINTF = $(PATH_LIB)/printf

INC = -I$(PATH_INC) -I$(PATH_LIBFT)/inc -I$(PATH_PRINTF)/includes

CC = clang
CFLAGS = -Werror -Wextra -Wall $(INC)

SRC_COMMON += arch_info.c
SRC_COMMON += error.c
SRC_COMMON += fat.c
SRC_COMMON += file.c
SRC_COMMON += mach.c
SRC_COMMON += section.c
SRC_COMMON += static_lib.c
SRC_COMMON += read_file.c
SRC_COMMON += symbol.c
SRC_COMMON += symbol_sort.c
SRC_COMMON += parse_commands.c
SRC_COMMON += parse_symtab.c
SRC_COMMON += print.c
SRC_COMMON += util_mem.c
SRC_COMMON += util_swap.c

SRC_NM += main.c
SRC_NM += print_nm.c

SRC_OTOOL += main.c
SRC_OTOOL += print_otool.c

OPATH_COMMON = $(PATH_OBJ)/$(PATH_COMMON)
SRCS_COMMON = $(addprefix $(PATH_SRC)/$(PATH_COMMON)/,$(SRC_COMMON))
OBJ_COMMON = $(notdir $(SRCS_COMMON:.c=.o))
OBJS_COMMON = $(addprefix $(PATH_OBJ)/$(PATH_COMMON)/,$(OBJ_COMMON))

OPATH_NM = $(PATH_OBJ)/$(PATH_NM)
SRCS_NM = $(addprefix $(PATH_SRC)/$(PATH_NM)/,$(SRC_NM))
OBJ_NM = $(notdir $(SRCS_NM:.c=.o))
OBJS_NM = $(addprefix $(OPATH_NM)/,$(OBJ_NM))

OPATH_OTOOL = $(PATH_OBJ)/$(PATH_OTOOL)
SRCS_OTOOL = $(addprefix $(PATH_SRC)/$(PATH_OTOOL)/,$(SRC_OTOOL))
OBJ_OTOOL = $(notdir $(SRCS_OTOOL:.c=.o))
OBJS_OTOOL = $(addprefix $(OPATH_OTOOL)/,$(OBJ_OTOOL))

LIBS = -L$(PATH_LIBFT)/ -lft -L$(PATH_PRINTF)/ -lftprintf

all: libs ft_nm ft_otool
	@echo ""

ft_nm: folders $(OBJS_COMMON) $(OBJS_NM)
	@echo ""
	@$(CC) -o $(NAME_NM) $(OBJS_COMMON) $(OBJS_NM) $(LIBS)
	@echo "[$(GREEN)ft_nm compiled!$(RESET)]"

ft_otool: folders $(OBJS_COMMON) $(OBJS_OTOOL)
	@echo ""
	@$(CC) -o $(NAME_OTOOL) $(OBJS_COMMON) $(OBJS_OTOOL) $(LIBS)
	@echo "[$(GREEN)ft_otool compiled!$(RESET)]"

libs:
	@make -C $(PATH_LIBFT)
	@make -C $(PATH_PRINTF)

folders:
	@mkdir -p $(OPATH_COMMON)
	@mkdir -p $(OPATH_OTOOL)
	@mkdir -p $(OPATH_NM)

$(OBJS_COMMON): $(PATH_OBJ)/$(PATH_COMMON)/%.o: $(PATH_SRC)/$(PATH_COMMON)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf .

$(OBJS_NM): $(PATH_OBJ)/$(PATH_NM)/%.o: $(PATH_SRC)/$(PATH_NM)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf .

$(OBJS_OTOOL): $(PATH_OBJ)/$(PATH_OTOOL)/%.o: $(PATH_SRC)/$(PATH_OTOOL)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf .

fcleanlibs:
	@make -C $(PATH_LIBFT) fclean
	@make -C $(PATH_PRINTF) fclean

clean:
	@rm -rf $(PATH_OBJ)

fclean: clean
	@rm -f $(NAME_NM)
	@rm -f $(NAME_OTOOL)
	@echo "[$(GREEN)cleaned$(RESET)]"

re: fclean all