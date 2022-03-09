SRCS =	./srcs/main.c ./srcs/config/create_simulation.c ./srcs/config/validate.c ./srcs/simulation/start_simulation.c \
		./srcs/checker/checker.c ./srcs/simulation/routine.c \
		./srcs/utils/ft_atoi.c ./srcs/utils/ft_calloc.c ./srcs/utils/ft_strlen.c ./srcs/utils/is_number.c ./srcs/utils/time.c

OBJS = $(SRCS:.c=.o)

NAME = philo

CC = gcc

CFLAGS = -pthread -g

all: $(NAME)

.c.o:
	${CC} ${CFLAGS} -I./includes -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME);

re: fclean all

run: all
	./$(NAME)
.PHONY: re fclean clean all