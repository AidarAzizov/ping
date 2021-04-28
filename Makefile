NAME = ft_ping


FLAGS = -Wall -Wextra -Werror

FUNC = helpers helpers_other main packet parse ping prepare print sendrecv socket lib

HEADER = -I includes/

SRC = $(addprefix src/, $(addsuffix .c, $(FUNC)))

OBJ = $(addprefix build/, $(addsuffix .o, $(FUNC)))


all: .PHONY $(NAME)

.PHONY:
	@mkdir -p build

$(NAME): $(OBJ)
	gcc $(FLAGS) -o $(NAME) $(OBJ) -lm $(HEADER)

build/%.o: srcs/%.c includes/ft_ping.h
	@gcc $(FLAGS) -o $@ -lm -c $< $(HEADER)

clean:
	/bin/rm -f $(OBJ)
	rm -rf build

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all