CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic -g

# Build the main dictionary executable
freq: dicttests.o dict.o 
	gcc $(CFLAGS) -o freq dicttests.o dict.o

debug: dicttests.o dict.o 
	gcc $(CFLAGS) -o freq dicttests.o dict.o

dicttests.o: dicttests.c dict.h
	gcc $(CFLAGS) -c dicttests.c 

dict.o: dict.c dict.h
	gcc $(CFLAGS) -c dict.c 

clean:
	rm freq dicttests.o dict.o

# Run Valgrind to check for memory leaks
grind: freq
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./freq in2.txt

gdb: freq
	gdb ./freq

test: freq
	./freq in1.txt