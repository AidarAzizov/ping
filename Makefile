NAME = ft_ping


FLAGS = -Wall -Wextra -Werror -lm

FUNC = main ping prepare print helpers helpers_other packet parse print sendrecv socket

HEADER = -I includes/

SRC = $(addprefix src/, $(addsuffix .c, $(FUNC)))

OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))


all: .PHONY $(NAME)

.PHONY:
	@mkdir -p build

$(NAME): $(OBJ)
	@gcc $(FLAGS) -o $(NAME) $(OBJ) $(HEADER)
	@echo "(•̀ᴗ•́)و $(NAME) generated!"

build/%.o: srcs/%.c includes/ft_ping.h
	@gcc $(FLAGS) -o $@ -c $< $(HEADER)

norm :
	@norminette $(SRC)

clean:
	@/bin/rm -f $(OBJ)
	@rm -rf build
	@echo "  OBJ files have been deleted."

fclean: clean
	@make fclean -C libft
	@/bin/rm -f $(NAME)
	@echo "  $(NAME) have been deleted."

re: fclean all