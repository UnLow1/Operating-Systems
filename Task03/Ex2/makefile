COMPILE = gcc -I. -Wall

program: 
	$(COMPILE) demo.c -o demo
	$(COMPILE) limit_test.c -o limit_test
	$(COMPILE) main.c -o main

.PHONY: clean

clean: 
	rm -f main demo limit_test