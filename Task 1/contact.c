#include <stdio.h>
#include <malloc.h>
#include "contact.h"

bool compare(Contact *a, Contact *b) {
    return a->firstName == b->firstName &&
           a->lastName == b->lastName &&
           a->birthday == b->birthday &&
           a->email == b->email &&
           a->phone == b->phone &&
           a->address == b->address;
}

//void printContact(Contact *contact) {
//    printf("\nfirstName: %s\n", contact->firstName);
//    printf("latName: %s\n", contact->lastName);
//    printf("birthday: %s\n", contact->birthday);
//    printf("email: %s\n", contact->email);
//    printf("phone: %s\n", contact->phone);
//    printf("address: %s\n", contact->address);
//}

Contact *enterContact(FILE *file) {

    Contact *contact = (Contact *) malloc(sizeof(Contact));

    fscanf(file, "%s", contact->firstName);
    fscanf(file, "%s", contact->lastName);
    fscanf(file, "%s", contact->birthday);
    fscanf(file, "%s", contact->email);
    fscanf(file, "%s", contact->phone);
    fscanf(file, "%s", contact->address);

    return contact;
}
