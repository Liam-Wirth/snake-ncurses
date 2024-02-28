// linkedlist.h

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {
    int x;
    int y;
    struct Node* next;
} Node;

Node* createNode(int x, int y);
void insertAtEnd(Node** headRef, int x, int y);
void freeList(Node* head);
void removeTail(Node** headRef);

#endif /* LINKEDLIST_H */

