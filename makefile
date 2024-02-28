CC = gcc
CFLAGS = -Wall -Wextra linkedlist.c
LDFLAGS = -lncurses

snake: snake.o
	$(CC) $(CFLAGS) -o snake snake.o $(LDFLAGS)

snake.o: snake.c
	$(CC) $(CFLAGS) -c snake.c linkedlist.c

clean:
	rm -f snake snake.o linkedlist.o
