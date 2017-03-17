#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"


void deleteAddressBookList(Node **head) {
    Node *tmp = *head;
    while (*head != NULL) {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

void addContactToList(Node **head, Contact *contact) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->next = NULL;
    node->contact = contact;

    if (*head == NULL) {
        node->prev = NULL;
        *head = node;
    } else {
        Node *tmp = *head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = node;
        node->prev = tmp;
    }
}

void deleteContactFromList(Node **head, Contact *contact) {
    Node *tmp = *head;
    while (tmp != NULL) {
        if (compare(tmp->contact, contact)) {
            if (tmp->prev != NULL && tmp->next != NULL) {
                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;
            } else if (tmp->prev == NULL) { // Deleting first element in list
                if (tmp->next != NULL) {
                    *head = tmp->next;
                    tmp->next->prev = NULL;
                } else
                    *head = NULL;
            } else // head->next == NULL
                tmp->prev->next = NULL;
            free(tmp);
            break;
        }
        tmp = tmp->next;
    }
}

Node *searchContactInList(Node *head, Contact *contact) {
    Node *result = NULL;
    while (head != NULL) {
        if (compare(head->contact, contact)) {
            result = head;
            break;
        }
        head = head->next;
    }
    return result;
}

int sizeOfAddressBookList(Node *head) {
    int size = 0;
    while (head != NULL) {
        size++;
        head = head->next;
    }
    return size;
}

void sortList(Node **head, enum option option) {
    if (*head == NULL || (*head)->next == NULL)// 0 or 1 element list - not sorting
        return;
    Node *i = (*head)->next;
    Node *tmp;
    Node *helper;

    while (i != NULL) {
        tmp = i;
        bool change;
        change = checkSortOptionList(&option, tmp);

        while (change) {
            helper = tmp->prev->prev;
            if (helper != NULL)
                helper->next = tmp;
            tmp->prev->next = tmp->next;
            tmp->prev->prev = tmp;
            if (tmp->next != NULL)
                tmp->next->prev = tmp->prev;
            tmp->next = tmp->prev;
            tmp->prev = helper;
            change = checkSortOptionList(&option, tmp);

        }
        //changing first element = change *head
        if (tmp->prev == NULL)
            *head = tmp;

        i = i->next;
    }
}

bool checkSortOptionList(enum option *option, Node *tmp) {
    bool change;
    switch ((*option)) {
        case lastName:
            change = tmp->prev != NULL && strcmp(tmp->contact->lastName, tmp->prev->contact->lastName) < 0;
            break;
        case birthday:
            change = tmp->prev != NULL && strcmp(tmp->contact->birthday, tmp->prev->contact->birthday) < 0;
            break;
        case email:
            change = tmp->prev != NULL && strcmp(tmp->contact->email, tmp->prev->contact->email) < 0;
            break;
        case phone:
            change = tmp->prev != NULL && strcmp(tmp->contact->phone, tmp->prev->contact->phone) < 0;
            break;
    }
    return change;
}

void printAddressBookList(Node *head) {
    while (head != NULL) {
        printf("\nfirstName: %s\n", head->contact->firstName);
        printf("latName: %s\n", head->contact->lastName);
        printf("birthday: %s\n", head->contact->birthday);
        printf("email: %s\n", head->contact->email);
        printf("phone: %s\n", head->contact->phone);
        printf("address: %s\n", head->contact->address);
        head = head->next;
    }
}
