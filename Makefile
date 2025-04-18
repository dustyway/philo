NAME	:= philo

CC	:= cc
CFLAGS	:= -g -fsanitize=thread #-Wextra -Wall -Werror

HEADERS	:= -I. 
SRCS	:= actions.c ft_list.c main.c parsing.c queue.c simulation.c utils.c
OBJS	:= ${SRCS:.c=.o}

all:  $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(HEADERS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re
