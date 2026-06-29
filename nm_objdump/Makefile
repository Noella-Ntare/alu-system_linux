CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRCS = hnm.c hnm-32.c hnm-32-helpers.c hnm-64.c hnm-64-helpers.c
OBJS = $(SRCS:.c=.o)
NAME = hnm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c hnm.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re hnm
