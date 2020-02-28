CC=gcc
CFLAGS=
LDFLAGS=
EXEC=miniShell

all: $(EXEC)

miniShell: miniShell shellEngine.o parser.o main.o
	$(CC) -o miniShell shellEngine.o parser.o main.o  -g

parser.o: parser.c
		$(CC) -o parser.o -c parser.c $(CFLAGS)

shellEngine.o: shellEngine.c parser.h
	$(CC) -o shellEngine.o -c shellEngine.c $(CFLAGS) -g

main.o: main.c shellEngine.h
	$(CC) -o main.o -c main.c $(CFLAGS) -g


run : 	
	./$(EXEC)

clean: clean
	rm -rf $(EXEC)
	rm -rf *.o