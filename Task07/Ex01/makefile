COMPILE = gcc -I. -Wall

all: program

program: 
	$(COMPILE) client.c -o client
	$(COMPILE) barber.c -o barber

.PHONY: clean

clean: 
	rm -f client barber
