LIB = ar rcs
RM = rm -f

RED    = \033[31m
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
RESET  = \033[0m

CC = cc
CCFLAGS = -Wall -Wextra -Werror -g3 -fPIE

NAME = libft.a

SRC = ft_memset.c ft_bzero.c ft_strlen.c ft_atoi.c ft_isdigit.c ft_isalpha.c\
ft_isprint.c ft_isascii.c ft_isalnum.c ft_memchr.c ft_memcpy.c\
ft_memcmp.c ft_memmove.c ft_strchr.c ft_strdup.c ft_strlcat.c ft_strlcpy.c\
ft_strncmp.c ft_toupper.c ft_tolower.c ft_strnstr.c ft_strrchr.c ft_calloc.c\
ft_putchar_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_putendl_fd.c ft_substr.c\
ft_strjoin.c ft_strtrim.c ft_strmapi.c ft_itoa.c ft_split.c ft_striteri.c\
get_next_line.c get_next_line_utils.c ft_strcount.c\
ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c\
ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c\
ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c\
ft_printf.c ft_intputnbr_fd.c ft_intputchar_fd.c ft_intputstr_fd.c ft_strtoupper.c \
ft_strtolower.c ft_long_hex.c ft_putpointer.c ft_ultoa.c ft_freedoublepointer.c\

OBJ = $(SRC:.c=.o)
INCLUDE = libft.h get_next_line.h ft_printf.h

all: $(NAME)

$(NAME): $(OBJ) $(INCLUDE)
	@echo "$(YELLOW)||>>   Compiling $(BLUE)libft $(YELLOW)with $(BLUE)printf $(YELLOW)and $(BLUE)gnl$(YELLOW)   <<||$(RESET)"
	@$(LIB) $(NAME) $(OBJ)
	@echo "$(YELLOW)       ||>>    $(BLUE)libft.a $(YELLOW)compiled!!    <<||$(RESET)"

%.o: %.c
	@$(CC) $(CCFLAGS) -c -o $@ $<

clean:
	@$(RM) $(OBJ)
	@echo "$(RED).o cleaned$(RESET)"

fclean:
	@$(RM) $(NAME) $(OBJ)
	@echo "$(RED).a and .o cleaned$(RESET)"

re: fclean all

.PHONY: all clean fclean re
