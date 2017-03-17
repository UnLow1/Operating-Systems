#ifndef LAB_1_BINTREE_H
#define LAB_1_BINTREE_H

#include "contact.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    Contact *contact;
    TreeNode *left;
    TreeNode *right;
};

void deleteAddressBookBinTree(TreeNode **root);

void addContactToBinTree(TreeNode **root, Contact *contact);

void deleteContactFromBinTree(TreeNode **root, Contact *contact);

TreeNode *searchContactBinTree(TreeNode *root, Contact *contact);

void sortBinTree(TreeNode **root, enum option option);

//void printAddressBookBinTree(TreeNode *root);

#endif //LAB_1_BINTREE_H
