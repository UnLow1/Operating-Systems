COMPILE = gcc -I. -Wall

program: 
	$(COMPILE) server.c -o server
	$(COMPILE) -fno-stack-protector  client.c -o client

.PHONY: clean

clean: 
	rm -f server client