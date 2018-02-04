##
## Makefile for mySlack in ~/mySlack For mySlack
##
## Made by MARION Tristan
## Login   <marion_t@etna-alternance.net>
##
## Started on  Sun Feb  4 11:19:36 2018 MARION Tristan
## Last update Sun Feb  6 11:10:41 2018 MARION Tristan
##

CC		= gcc
NAME		= server
BUILD_PATH	= obj
SRC		= src/server.c src/main.c
INCLUDES	= -I src/includes
CFLAGS		= -W -Wextra -Werror -Wall -pedantic $(INCLUDES)
OBJ		= $(SRC:%.c=$(BUILD_PATH)/%.o)

MKDIR	= mkdir
RM	= rm -f
RMDIR	= rm -rf

.PHONY	: all dirs clean fclean re

all	: $(NAME)

$(NAME)	: dirs $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

dirs	:
	$(MKDIR) -p $(dir $(OBJ))

$(BUILD_PATH)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(RM) $(OBJ)
	$(RMDIR) $(dir $(OBJ))

fclean	: clean
	$(RM) $(NAME)

re	: fclean all
