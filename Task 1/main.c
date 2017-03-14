#include <stdio.h>
#include <stdlib.h>
#include "binTree.h"
#include "list.h"


int main() {
//    Node *head = NULL;
    TreeNode *root = NULL;
    FILE *file = fopen("dane.txt", "r");
    while (!feof(file)) {
        Contact *contact = (Contact *) malloc(sizeof(Contact));
        fscanf(file, "%s", contact->firstName);
        fscanf(file, "%s", contact->lastName);
        fscanf(file, "%s", contact->birthday);
        fscanf(file, "%s", contact->email);
        fscanf(file, "%s", contact->phone);
        fscanf(file, "%s", contact->address);
        addContactToBinTree(&root, contact);
//        addContactToList(&head, contact);
    }
    fclose(file);

    printf("\nPRINTING ALL TREE\n");
    printAddressBookBinTree(root);
    printf("\nSEARCHING ONE ELEMENT\n");
    printContact(searchContactBinTree(root,root->contact)->contact);
    printf("\nSORTING BY PHONE\n");
    sortBinTree(&root,phone);
    printAddressBookBinTree(root);
    printf("\nDELETING ROOT\n");
    deleteContactFromBinTree(&root,root->contact);
    printf("\nPRINTING ALL TREE (AFTER DELETING ONE ELEMET)\n");
    printAddressBookBinTree(root);
    printf("\nDELETING ALL TREE\n");
    deleteAddressBookBinTree(&root);
    printf("\nPRINTING ALL TREE (AFTER DELETE)\n");
    printAddressBookBinTree(root);

//    printf("Size = %d\n", sizeOfAddressBookList(head));
//    printf("\nPrinting all elements in list\n");
//    printAddressBookList(head);
//
//    printf("\nPrinting found element\n");
//    printContact(searchContactInList(head,head->contact)->contact);
//
//    printf("\nSorting by birthday\n");
//    sortList(&head, birthday);
//    printf("Printing all elements in list (after sorting by birthday)\n");
//    printAddressBookList(head);
//
//    printf("\nDeleting one element\n");
//    deleteContactFromList(&head, head->contact);
//    printf("Printing all elements in list (after deleting)\n");
//    printAddressBookList(head);
//    printf("\nDeleting AddressBook\n");
//    deleteAddressBookList(&head);
//    printf("Size = %d\n", sizeOfAddressBookList(head));

    return 0;
}





