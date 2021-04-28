NAME = ft_ping


FLAGS = -Wall -Wextra -Werror -lm

FUNC = helpers helpers_other main packet parse ping prepare print sendrecv socket lib

HEADER = -I includes/

SRC = $(addprefix src/, $(addsuffix .c, $(FUNC)))

OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))


all: .PHONY $(NAME)

.PHONY:
	@mkdir -p build

$(NAME): $(OBJ)
	gcc $(FLAGS) -o $(NAME) $(OBJ) $(HEADER)
	echo "(•̀ᴗ•́)و $(NAME) generated!"

build/%.o: srcs/%.c includes/ft_ping.h
	gcc $(FLAGS) -o $@ -c $< $(HEADER)

norm :
	norminette $(SRC)

clean:
	/bin/rm -f $(OBJ)
	rm -rf build
	echo "  OBJ files have been deleted."

fclean: clean
	/bin/rm -f $(NAME)
	echo "  $(NAME) have been deleted."

re: fclean all