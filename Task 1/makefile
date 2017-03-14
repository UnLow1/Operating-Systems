COMPILE=gcc -I. -Wall -std=c99

all: makeContactBook static_library dynamic_library

makeContactBook: main.c binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -o contactBook main.c binTree.c contact.c list.c

static_library: main.c binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -c main.c 
	$(COMPILE) -c binTree.c 
	$(COMPILE) -c contact.c 
	$(COMPILE) -c list.c 
	ar rcsv library_static.a main.o binTree.o contact.o list.o	
	
dynamic_library: main.c binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -c -fPIC main.c 
	$(COMPILE) -c -fPIC binTree.c 
	$(COMPILE) -c -fPIC contact.c 
	$(COMPILE) -c -fPIC list.c 
	$(COMPILE) -shared -fPIC main.o binTree.o contact.o list.o -o library_dynamic.so
