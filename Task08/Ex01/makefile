COMPILE = gcc -I. -Wall -pthread

all: program

program: 
	$(COMPILE) main1.c -o main1
	$(COMPILE) main2.c -o main2
	$(COMPILE) main3.c -o main3


.PHONY: clean

clean: 
	rm -f main1 main2 main3