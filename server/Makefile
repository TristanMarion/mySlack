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
SRC		= src/server.c src/main.c src/command.c src/server_command.c src/sending.c \
			src/init_client.c src/init_server.c src/manage_lists.c src/config.c  src/getters.c \
			src/commands/commands_channel.c \
			../common/libmy.c ../common/color.c ../common/display.c  ../common/common.c
OBJ		= $(SRC:%.c=%.o)
INCLUDES	= -I ../includes -I ./includes
CFLAGS		= -W -Wextra -Werror -Wall -pedantic $(INCLUDES)

MKDIR	= mkdir
RM	= rm -f
RMDIR	= rm -rf

.PHONY	: all dirs clean fclean re

all	: $(NAME)

$(NAME)	: dirs $(OBJ)
	$(CC) $(CFLAGS) $(addprefix $(BUILD_PATH)/, $(notdir $(OBJ))) -o $(NAME)

dirs	:
	$(MKDIR) -p $(BUILD_PATH)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$(notdir $@)

clean	:
	$(RM) $(addprefix $(BUILD_PATH)/, $(notdir $(OBJ)))
	$(RMDIR) $(BUILD_PATH)

fclean	: clean
	$(RM) $(NAME)

re	: fclean all
