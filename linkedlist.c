// linkedlist.c

#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

Node* createNode(int x, int y) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

void insertAtEnd(Node** headRef, int x, int y) {
    Node* newNode = createNode(x, y);
    if (*headRef == NULL) {
        *headRef = newNode;
    } else {
        Node* current = *headRef;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

void removeTail(Node** headRef) {
    if (*headRef == NULL) {
        return;
    } else if ((*headRef)->next == NULL) {
        free(*headRef);
        *headRef = NULL;
    } else {
        Node* current = *headRef;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
}

