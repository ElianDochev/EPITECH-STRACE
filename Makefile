##
## EPITECH PROJECT, 2023
## strace Makefile
## File description:
## compiles and builds lib and the source files runs unit tests
##

NAME	=	strace

SRC	=	##

SRC_MAIN = src/main.c

DEBUG = -g3 -Wall -Wextra

CPPFLAGS = -I./headers

OBJ	=	$(SRC:.c=.o)

CC = gcc

OBJ_MAIN = $(SRC_MAIN:.c=.o)

LIB_DIR = ../lib/

CLIB 	= -L$(LIB_DIR) -lmy

TESTS = ##tests/

TEST_OBJ = $(TESTS:.c=.o)

LIB_TEST = ar rc -o libutest.a

TEST_INCLUDE = -L./ -lmy --coverage -lcriterion

TEST_NAME = unit_test

RM = rm -f

all: compile

debug:
	$(MAKE) CPPFLAGS="$(CPPFLAGS) $(DEBUG)" compile

compile: $(OBJ) $(OBJ_MAIN)
	$(CC) -o $(NAME) $(OBJ) $(OBJ_MAIN) $(CLIB)

makelib:
	$(MAKE) -C $(LIB_DIR)

clean:
	$(RM) $(OBJ) $(OBJ_MAIN)

fclean: clean
	$(RM) $(NAME)

re: fclean all

unit_tests: fclean $(OBJ) $(TEST_OBJ)
		$(MAKE) -C $(LIB_DIR)
		$(LIB_TEST) $(OBJ)
		$(CC) -o $(TEST_NAME) $(TESTS) $(TEST_INCLUDE)

tests_run: unit_tests
			./$(TEST_NAME)

unit_clean: fclean
			$(RM) $(LIB_TEST)
			$(RM) unit_test*
			$(RM) $(TEST_OBJ)

.PHONY: all debug compile clean fclean re unit_tests tests_run unit_clean

