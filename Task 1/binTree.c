#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "binTree.h"

void addContactToBinTree(TreeNode **root, Contact *contact) {
    TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
    node->left = NULL;
    node->right = NULL;
    node->contact = contact;

    if (*root == NULL)
        *root = node;
    else {
        TreeNode *tmp = *root;
        while (true) {
            if (strcmp(contact->lastName, tmp->contact->lastName) < 0) {//contact->lastname < tmp->contact->lastname
                if (tmp->left != NULL)
                    tmp = tmp->left;
                else {
                    tmp->left = node;
                    break;
                }
            } else { //contact->lastname > tmp->contact->lastname
                if (tmp->right != NULL)
                    tmp = tmp->right;
                else {
                    tmp->right = node;
                    break;
                }
            }
        }
    }
}

void printAddressBookBinTree(TreeNode *root) {
    if (root != NULL) {
        printAddressBookBinTree(root->left);
        printContact(root->contact);
        printAddressBookBinTree(root->right);
    }
}

void deleteAddressBookBinTree(TreeNode **root) {
    if (*root == NULL)
        return;
    TreeNode *left = (*root)->left;
    TreeNode *right = (*root)->right;
    free(*root);
    *root = NULL;
    deleteAddressBookBinTree(&left);
    deleteAddressBookBinTree(&right);
}

TreeNode *searchContactBinTree(TreeNode *root, Contact *contact) {
    TreeNode *result = NULL;
    if (root == NULL)
        return NULL;
    if (compare(root->contact, contact))
        return root;
    if (searchContactBinTree(root->left, contact) != NULL)
        result = searchContactBinTree(root->left, contact);
    if (result == NULL && searchContactBinTree(root->right, contact) != NULL)
        result = searchContactBinTree(root->right, contact);
    return result;
}

void deleteContactFromBinTree(TreeNode **root, Contact *contact) {
    if ((*root) == NULL)
        return;
    TreeNode *toDelete = *root;
    if (compare((*root)->contact, contact)) {
        if ((*root)->left == NULL) {
            if ((*root)->right == NULL) {
                *root = NULL;   // Tree is now empty
            } else {
                *root = (*root)->right;
            }
        } else {
            TreeNode *helper = *root;
            TreeNode *tmp;
            while (helper->left->left != NULL)
                helper = helper->left;
            tmp = helper->left->right;
            if (helper != (*root))
                helper->left->left = (*root)->left;
            else
                helper->left->left = NULL;
            helper->left->right = (*root)->right;
            *root = helper->left;
            helper->left = tmp;
        }
        free(toDelete);
    } else {
        deleteContactFromBinTree(&(*root)->left, contact);
        deleteContactFromBinTree(&(*root)->right, contact);
    }

}

void sortBinTree(TreeNode **root, enum option option) {
    TreeNode *newRoot = NULL;
    while ((*root) != NULL) {


        TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
        node->left = NULL;
        node->right = NULL;
        node->contact = (*root)->contact;
        if (newRoot == NULL)
            newRoot = node;
        else {
            bool goLeft;
            TreeNode *tmp = newRoot;
            while (true) {
                switch (option) {
                    case lastName:
                        goLeft = strcmp(node->contact->lastName, tmp->contact->lastName) < 0;
                        break;
                    case birthday:
                        goLeft = strcmp(node->contact->birthday, tmp->contact->birthday) < 0;
                        break;
                    case email:
                        goLeft = strcmp(node->contact->email, tmp->contact->email) < 0;
                        break;
                    case phone:
                        goLeft = strcmp(node->contact->phone, tmp->contact->phone) < 0;
                        break;
                }
                if (goLeft) {//contact->lastname < tmp->contact->lastname
                    if (tmp->left != NULL)
                        tmp = tmp->left;
                    else {
                        tmp->left = node;
                        break;
                    }
                } else { //contact->lastname > tmp->contact->lastname, goLeft == false
                    if (tmp->right != NULL)
                        tmp = tmp->right;
                    else {
                        tmp->right = node;
                        break;
                    }
                }
            }
        }

        if ((*root)->left == NULL) {
            if ((*root)->right == NULL) {
                *root = NULL;   // Tree is now empty
            } else {
                *root = (*root)->right;
            }
        } else {
            TreeNode *helper = *root;
            TreeNode *tmp;
            while (helper->left->left != NULL)
                helper = helper->left;
            tmp = helper->left->right;
            if (helper != (*root))
                helper->left->left = (*root)->left;
            else
                helper->left->left = NULL;
            helper->left->right = (*root)->right;
            *root = helper->left;
            helper->left = tmp;
        }
    }
    *root = newRoot;
}
