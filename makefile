FILES=raylib/lib/libraylib.a
FLAGS=-I. -Iraylib/include/ -ldl -lpthread -lm
DEBUG=-Wextra -Wall -Werror -pedantic -g -fsanitize=address

all:
	mkdir -p bin
	$(CC) main.c $(FLAGS) $(FILES) $(DEBUG) -o bin/game
	clear
	./bin/game

.PHONY: all
