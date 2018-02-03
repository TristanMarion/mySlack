SRC	=	server.c main.c
OBJ	=	$(SRC:%.c=%.o)
NAME	=	server
CC	=	gcc
CFLAGS = -W -Wall -Werror -Wextra
RM	= rm -f

all: $(NAME)

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
