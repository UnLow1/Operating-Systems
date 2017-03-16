#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <afxres.h>
#include "binTree.h"
#include "list.h"
#include <dlfcn.h>


LARGE_INTEGER getFILETIMEoffset() {
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(int X, struct timeval *tv) {
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset;
    static double frequencyToMicroseconds;
    static int initialized = 0;
    static BOOL usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double) performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double) t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}

void printTime(struct timeval start_time, struct timeval stop_time) {
    printf("%ld seconds and %ld microseconds\n", (stop_time.tv_sec - start_time.tv_sec),
           (stop_time.tv_usec - start_time.tv_usec));
}


int main() {
    // Opening the library
    void *handle = dlopen("./libshared.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // Getting the functions
    void (*addContactToList)(Node **head, Contact *contact)=dlsym(handle, "addContactToList");
    void (*deleteAddressBookList)(Node **head)=dlsym(handle, "deleteAddressBookList");
    void (*deleteContactFromList)(Node **head, Contact *contact)=dlsym(handle, "deleteContactFromList");
    Node *(*searchContactInList)(Node *head, Contact *contact)=dlsym(handle, "searchContactInList");
    int (*sizeOfAddressBookList)(Node *head)=dlsym(handle, "sizeOfAddressBookList");
    void (*sortList)(Node **head, enum option option)=dlsym(handle, "sortList");
    bool (*checkSortOptionList)(enum option *option, Node *tmp)=dlsym(handle, "checkSortOptionList");
    void (*printAddressBookList)(Node *head)=dlsym(handle, "printAddressBookList");
    void (*deleteAddressBookBinTree)(TreeNode **root)=dlsym(handle, "deleteAddressBookBinTree");
    void (*addContactToBinTree)(TreeNode **root, Contact *contact)=dlsym(handle, "addContactToBinTree");
    void (*deleteContactFromBinTree)(TreeNode **root, Contact *contact)=dlsym(handle, "deleteContactFromBinTree");
    TreeNode *(*searchContactBinTree)(TreeNode *root, Contact *contact)=dlsym(handle, "searchContactBinTree");
    void (*sortBinTree)(TreeNode **root, enum option option)=dlsym(handle, "sortBinTree");
    void (*printAddressBookBinTree)(TreeNode *root)=dlsym(handle, "printAddressBookBinTree");
    bool (*compare)(Contact *a, Contact *b)=dlsym(handle, "compare");
    void (*printContact)(Contact *contact)=dlsym(handle, "printContact");
    Contact *(*enterContact)(FILE *file)=dlsym(handle, "enterContact");

    struct timeval start_time;
    struct timeval stop_time;
    struct timeval start_time_makeContactBook;
    struct timeval stop_time_makeContactBook;

    printf("\n\n LIST\n\n");

    clock_gettime(0, &start_time_makeContactBook);
    Node *head = NULL;
    FILE *file = fopen("data_generator.txt", "r");
    while (!feof(file)) {
        clock_gettime(0, &start_time);
        Contact *contact = enterContact(file);
        addContactToList(&head, contact);
        clock_gettime(0, &stop_time);
//        printf("Adding contact: ");
//        printTime(start_time, stop_time);
    }
    fclose(file);
    clock_gettime(0, &stop_time_makeContactBook);
    printf("Making contactBook: ");
    printTime(start_time_makeContactBook, stop_time_makeContactBook);

    //    SORTING LIST
    clock_gettime(0, &start_time);
    sortList(&head, lastName);
    clock_gettime(0, &stop_time);
    printf("Sorting list: ");
    printTime(start_time, stop_time);

    //MAKING BIN TREE (USING THEM LATER)
//    TreeNode *rootTmp = NULL;
//    Node *headTmp = head;
//    Contact *cont = NULL;
//    while (headTmp != NULL) {
//        addContactToBinTree(&rootTmp,headTmp->contact);
//        cont = headTmp->contact;
//        headTmp = headTmp->next;
//    }

    // FINDING ELEMENT
    clock_gettime(0, &start_time);
    Node *tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    searchContactInList(head, tmp->contact);
    clock_gettime(0, &stop_time);
    printf("Finding element (pesymistic): ");
    printTime(start_time, stop_time);

    clock_gettime(0, &start_time);
    searchContactInList(head, head->contact);
    clock_gettime(0, &stop_time);
    printf("Finding element (optimistic): ");
    printTime(start_time, stop_time);

    // DELETING ELEMENT
    clock_gettime(0, &start_time);
    tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    deleteContactFromList(&head, tmp->contact);
    clock_gettime(0, &stop_time);
    printf("Deleting element (pesymistic): ");
    printTime(start_time, stop_time);

    clock_gettime(0, &start_time);
    deleteContactFromList(&head, head->contact);
    clock_gettime(0, &stop_time);
    printf("Deleting element (optimistic): ");
    printTime(start_time, stop_time);

    // DELETING ADDRESSBOOK
    clock_gettime(0, &start_time);
    deleteAddressBookList(&head);
    clock_gettime(0, &stop_time);
    printf("Deleting AddressBook: ");
    printTime(start_time, stop_time);

//    printf("\nPrinting all elements in list\n");
//    printAddressBookList(head);


    printf("\n\n BIN TREE\n\n");

    clock_gettime(0, &start_time_makeContactBook);
    TreeNode *root = NULL;
    file = fopen("data_generator.txt", "r");
    while (!feof(file)) {
        clock_gettime(0, &start_time);
        Contact *contact = enterContact(file);
        addContactToBinTree(&root, contact);
        clock_gettime(0, &stop_time);
//        printf("Adding contact: ");
//        printTime(start_time, stop_time);
    }
    fclose(file);
    clock_gettime(0, &stop_time_makeContactBook);
    printf("Making contactBook: ");
    printTime(start_time_makeContactBook, stop_time_makeContactBook);

    // SORTING BIN TREE
    clock_gettime(0, &start_time);
    sortBinTree(&root, birthday);
    clock_gettime(0, &stop_time);
    printf("Sorting binTree: ");
    printTime(start_time, stop_time);

//    // FINDING ELEMENT
//        clock_gettime(0, &start_time);
////    TreeNode *temporary = rootTmp;
////    while (temporary->right != NULL)
////        temporary = temporary ->right;
//    searchContactBinTree(rootTmp, cont);
//    clock_gettime(0, &stop_time);
//    printf("Finding element (pesymistic): ");
//    printTime(start_time, stop_time);

    clock_gettime(0, &start_time);
    searchContactBinTree(root, root->contact);
    clock_gettime(0, &stop_time);
    printf("Finding element (optimistic): ");
    printTime(start_time, stop_time);

    // DELETING ELEMENT
//    clock_gettime(0, &start_time);
//    temporary = rootTmp;
//    while (temporary -> right != NULL)
//        temporary = temporary -> right;
//    deleteContactFromBinTree(&rootTmp, temporary->contact);
//    clock_gettime(0, &stop_time);
//    printf("Deleting element (pesymistic): ");
//    printTime(start_time, stop_time);
//
    clock_gettime(0, &start_time);
    deleteContactFromBinTree(&root, root->contact);
    clock_gettime(0, &stop_time);
    printf("Deleting element (optimistic): ");
    printTime(start_time, stop_time);


    // DELETING ADDRESSBOOK
    clock_gettime(0, &start_time);
    deleteAddressBookBinTree(&root);
    clock_gettime(0, &stop_time);
    printf("Deleting AddressBook: ");
    printTime(start_time, stop_time);

//    printf("\nPRINTING ALL TREE\n");
//    printAddressBookBinTree(root);


//    printAddressBookBinTree(rootTmp);

    dlclose(handle);

    return 0;
}
