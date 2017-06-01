COMPILE = gcc -I. -Wall

all: program

program: 
	$(COMPILE) main.c -o main -lrt -lpthread

.PHONY: clean

clean: 
	rm -f main