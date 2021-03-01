CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

main: functions.o main.o
	$(CC) $(LDFLAGS) -o forests $^

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o forests
