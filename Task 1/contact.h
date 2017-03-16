#ifndef LAB_1_CONTACT_H
#define LAB_1_CONTACT_H

#include <stdbool.h>

#define SIZE 25

typedef enum option option;

enum option {
    lastName, birthday, email, phone
};

typedef struct Contact Contact;

struct Contact {
    char firstName[SIZE];
    char lastName[SIZE];
    char birthday[SIZE];
    char email[SIZE];
    char phone[SIZE];
    char address[SIZE];
};

bool compare(Contact *a, Contact *b);

void printContact(Contact *contact);

Contact *enterContact();


#endif //LAB_1_CONTACT_H
