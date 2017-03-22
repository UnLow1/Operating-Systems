COMPILE = gcc -I. -Wall

all: program

program: 
	$(COMPILE) main.c fileInfo.c inOutLibrary.c inOutSystems.c -o main

.PHONY: clean

clean: 
	rm -f main