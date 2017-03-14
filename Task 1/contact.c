//
// Created by Admin on 13.03.2017.
//

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

void printContact(Contact *contact) {
    printf("\nfirstName: %s\n", contact->firstName);
    printf("latName: %s\n", contact->lastName);
    printf("birthday: %s\n", contact->birthday);
    printf("email: %s\n", contact->email);
    printf("phone: %s\n", contact->phone);
    printf("address: %s\n", contact->address);
}

Contact *enterContact() {

    Contact *contact = (Contact *) malloc(sizeof(Contact));

    printf("Enter a firstName: ");
    scanf("%s", contact->firstName);

    printf("Enter a lastName: ");
    scanf("%s", contact->lastName);

    printf("Enter a birthday: ");
    scanf("%s", contact->birthday);

    printf("Enter a email: ");
    scanf("%s", contact->email);

    printf("Enter a phone: ");
    scanf("%s", contact->phone);

    printf("Enter a address: ");
    scanf("%s", contact->address);

    return contact;
}