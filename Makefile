NAME = ft_ping

PATH_SRC = ./srcs/

PATH_INC = ./includes/

INCLUDES = -I $(PATH_INC)

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) -g
LIBS = -lm

SRC =	main.c \
	ping.c \
	prepare.c \
	print.c \
	helpers.c \
	helpers_other.c \
	packet.c \
	parse.c \
	print.c \
	sendrecv.c \
	socket.c

OBJ = $(addprefix $(PATH_SRC), $(SRC:.c=.o))

.PHONY: clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft/
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
