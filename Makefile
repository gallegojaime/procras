CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter
NAME = stprocras-list

$(NAME) : $(NAME).c
	$(CC) $(CFLAGS) -o $@  $@.c

memcheck : $(NAME)
	valgrind --track-origins=yes ./$(NAME)

clean :
	rm $(NAME)
