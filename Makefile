NAME = mini
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
CLIBS = -lreadline

SRC_PATH = src/
OBJ_PATH = obj/
LIB_PATH = ./libft_ext/

SRC =	minish.c \
		minish2.c \
		minish3.c \
		minish4.c \
		minish5.c \
		minish6.c \
		minish7.c \
		minish8.c \
		pipex.c \
		pipex2.c \
		pipex3.c \
		pipex4.c \
		ft_splitchars.c \
		signals.c \
		builtins.c \
		environment1.c \
		environment2.c

SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJS = $(patsubst $(SRC_PATH)%.c,$(OBJ_PATH)%.o,$(SRCS))
INC = -I/usr/include/readline -I/usr/include -I ./inc/
LIB = $(addprefix $(LIB_PATH), libft.a)

GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m


all: $(NAME)

$(LIB):
	@printf "$(YELLOW)Building libft...$(RESET) \n"
	@$(MAKE) -C $(LIB_PATH) > /dev/null 2>&1 && \
		printf "$(GREEN)✔ Build succesful!$(RESET) \n" || \
		printf "$(RED)✘ Build failed!$(RESET) \n"

$(NAME): $(MLX) $(LIB) $(OBJS)
	@printf "$(YELLOW)Building $(NAME)...$(RESET) \n"
	@$(CC) $(CFLAGS) $(OBJS) $(INC) $(MLX) $(LIB) $(CLIBS) -o $(NAME) && \
		printf "$(GREEN)✔ Build succesful!$(RESET) \n" || \
		printf "$(RED)✘ Build failed!$(RESET) \n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c | $(OBJ_PATH)
	@printf "$(BLUE)Compiling $<...$(RESET) \n"
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJ_PATH):
	@printf "$(BLUE)Creating object directory...$(RESET) \n"
	@mkdir -p $(OBJ_PATH)

clean:
	@printf "$(BLUE)Cleaning object files...$(RESET) \n"
	@$(MAKE) -C $(LIB_PATH) clean > /dev/null 2>&1
	@rm -f $(OBJS) $(BOBJS)
	@printf "$(GREEN)✔ Objects cleaned succesfully!$(RESET) \n"

fclean: clean
	@printf "$(BLUE)Removing binaries, dependencies and object files...$(RESET) \n"
	@$(MAKE) -C $(LIB_PATH) fclean > /dev/null 2>&1
	@rm -f $(NAME)
	@rm -rf $(MLX_PATH)
	@printf "$(GREEN)✔ Directory cleaned succesfully!$(RESET) \n"

re: fclean all

call: all clean
	@printf "$(YELLOW)Cleaning dependency builds...$(RESET) \n"
	@$(MAKE) -C $(LIB_PATH) fclean > /dev/null 2>&1
	@rm -rf $(MLX_PATH)
	@printf "$(GREEN)✔ Dependency builds cleaned succesfully!$(RESET) \n"

.PHONY: all clean fclean re call
