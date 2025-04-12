CC = gcc
CFLAGS = -g -Wall

OBJS = mysh.o engine.o

all: mysh

mysh: $(OBJS)
	$(CC) $(CFLAGS) -o mysh $(OBJS)

mysh.o: mysh.c shcore.h
shcore.o: shcore.c shcore.h

clean:
	rm -f *.o mysh
