#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include "binTree.h"
#include "list.h"

void timeCheckpoint(const char *message) {
    static struct timespec previous_real_time;
    // Getting time values
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct timeval user_time = usage.ru_utime;
    struct timeval system_time = usage.ru_stime;
    struct timespec real_time;
    clock_gettime(CLOCK_REALTIME, &real_time);
    // Show info
    printf("real time: %lld | user time: %ld | system time: %ld  ---  %s\n",
           (long long)(real_time.tv_sec-previous_real_time.tv_sec)*1000000+(real_time.tv_nsec-previous_real_time.tv_nsec)/1000,
           user_time.tv_sec*1000000+user_time.tv_usec,
           system_time.tv_sec*1000000+system_time.tv_usec,
           message
    );
    previous_real_time = real_time;
}

int main() {

    printf("\n\n LIST\n\n");

    timeCheckpoint("Starting program");

    Node *head = NULL;
    FILE *file = fopen("data_generator.txt", "r");
    while (!feof(file)) {
        Contact *contact = enterContact(file);
        addContactToList(&head, contact);
    }
    fclose(file);
    timeCheckpoint("Making contactBook");


    //    SORTING LIST

    sortList(&head, lastName);
    timeCheckpoint("Sorting list");

    // FINDING ELEMENT

    Node *tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    searchContactInList(head, tmp->contact);
    timeCheckpoint("Finding element (pesymistic)");



    searchContactInList(head, head->contact);
    timeCheckpoint("Finding element (optimistic)");


    // DELETING ELEMENT

    tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    deleteContactFromList(&head, tmp->contact);
    timeCheckpoint("Deleting element (pesymistic)");



    deleteContactFromList(&head, head->contact);
    timeCheckpoint("Deleting element (optimistic)");


    // DELETING ADDRESSBOOK

    deleteAddressBookList(&head);
    timeCheckpoint("Deleting AddressBook");

    printf("\n\n BIN TREE\n\n");


    TreeNode *root = NULL;
    file = fopen("data_generator.txt", "r");
    while (!feof(file)) {

        Contact *contact = enterContact(file);
        addContactToBinTree(&root, contact);
    }
    fclose(file);
    timeCheckpoint("Making contactBook");


    // SORTING BIN TREE

    sortBinTree(&root, birthday);
    timeCheckpoint("Sorting binTree");


   // FINDING ELEMENT (OPTIMISTIC)

    searchContactBinTree(root, root->contact);
    timeCheckpoint("Finding element (optimistic)");


    // DELETING ELEMENT (OPTIMISTIC)

    deleteContactFromBinTree(&root, root->contact);
    timeCheckpoint("Deleting element (optimistic)");

    // DELETING ADDRESSBOOK

    deleteAddressBookBinTree(&root);

    timeCheckpoint("Deleting AddressBook");


    printf("\n\n");
    //MAKING BIN TREE (USING THEM LATER)
    head = NULL;
    file = fopen("data_generator.txt", "r");
    while (!feof(file)) {
        Contact *contact = enterContact(file);
        addContactToList(&head, contact);
    }
    fclose(file);
    sortList(&head, lastName);

    root = NULL;
    Contact *cont = NULL;
    while (head != NULL) {
        addContactToBinTree(&root,head->contact);
        cont = head->contact;
        deleteContactFromList(&head,head->contact);
    }
    timeCheckpoint("Making new tree for testing pesymistic options");

    deleteContactFromBinTree(&root, cont);
    timeCheckpoint("Deleting element (pesymistic)");

    searchContactBinTree(root, cont);
    timeCheckpoint("Finding element (pesymistic)");


    return 0;
}
