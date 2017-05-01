COMPILE = gcc -I. -Wall

program: 
	$(COMPILE) server.c -o server -lrt
	$(COMPILE) client.c -o client -lrt

.PHONY: clean

clean: 
	rm -f server client