COMPILE = gcc

all: program

program: 
	$(COMPILE) barber.c -o barber -lrt -lpthread
	$(COMPILE) client.c -o client -lrt -lpthread

.PHONY: clean

clean: 
	rm -f client barber
