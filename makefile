
CC = gcc
CCFLAGS = -Wall -Wextra -Werror

RM		= rm -f
SRC_DIR					= src
FILE_HANDLER_SRC_DIR	= FileHandler/src
ELF_PARSER_SRC_DIR		= ElfParser/src

NAME = nm



$(NAME): 
	${CC} ${CCFLAGS} -o ${NAME} ${SRC_DIR}/*  ${FILE_HANDLER_SRC_DIR}/* ${ELF_PARSER_SRC_DIR}/* 


all:  ${NAME}

clean:        
	${RM} ${MAIN_OBJ_FILES} ${BONUS_OBJ_FILES}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re 