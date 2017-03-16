COMPILE=gcc -I. -Wall -std=c99

all: makeContactBookWithStatic makeContactBookWithShared makeContactBook libcontactBook.a libcontactBook.so

makeContactBook: main.c binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -o contactBook main.c binTree.c contact.c list.c
	
makeContactBookWithStatic: main.c libcontactBook.a
	$(COMPILE) main.c -L. -lcontactBook -o contactBookWithStatic
	
makeContactBookWithShared: main.c libcontactBook.so
	$(COMPILE) main.c -L. -Wl,-rpath=. -lcontactBook -o contactBookWithShared
	


libcontactBook.a: binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -c binTree.c 
	$(COMPILE) -c contact.c 
	$(COMPILE) -c list.c 
	ar rcsv libcontactBook.a binTree.o contact.o list.o	
	
libcontactBook.so: binTree.c contact.c list.c binTree.h contact.h list.h
	$(COMPILE) -c -fPIC binTree.c 
	$(COMPILE) -c -fPIC contact.c 
	$(COMPILE) -c -fPIC list.c 
	$(COMPILE) -shared -fPIC binTree.o contact.o list.o -o libcontactBook.so

.PHONY: clean

clean:
	-rm  *.o
