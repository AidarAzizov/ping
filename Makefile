NAME = push_swap


FLAGS = -Wall -Wextra -Werror

FUNC = main ping prepare print helpers helpers_other packet parse print sendrecv socket

HEADER = -I includes/

SRC = $(addprefix src/, $(addsuffix .c, $(FUNC)))

OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))

#=====COLOUR========
RED = \033[1;31m
GREEN = \033[1;92m
BLUE = \033[34;1m
YELLOW = \033[0;33m
#===================

all: .PHONY $(NAME)

.PHONY:
    @mkdir -p build

$(NAME): $(OBJ)
	@echo "$(GREEN)(ﾉ◕ヮ◕)ﾉ*:・ﾟ✧ Compiling progect... Wait a sec."
	@gcc $(FLAGS) -o $(NAME) $(OBJ) $(HEADER)
	@echo "$(GREEN)(•̀ᴗ•́)و $(NAME) generated!"

build/%.o: src/%.c includes/push_swap.h
	@gcc $(FLAGS) -o $@ -c $< $(HEADER)

norm :
	@norminette $(SRC)

clean:
	@/bin/rm -f $(OBJ)
	@rm -rf build
	@echo "  $(BLUE)OBJ files have been deleted."

fclean: clean
	@make fclean -C libft
	@/bin/rm -f $(NAME)
	@echo "  $(BLUE)$(NAME) have been deleted."

re: fclean all