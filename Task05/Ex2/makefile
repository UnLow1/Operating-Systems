COMPILE = gcc -I. -Wall

program: 
	$(COMPILE) slave.c -o slave -lm
	$(COMPILE) master.c -o master

.PHONY: clean

clean: 
	rm -f slave master