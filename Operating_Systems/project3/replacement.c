#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include "list.h"

//struct Node *head=NULL;
int random()
{
		int next = rand() % NUM_FRAME;
		debug("next = %d\n", next);
		return next;
}

int fifo(struct Node **head)
{
    int ok;
struct Node *ret =*head;
while(ret->next!=NULL){
ret=ret->next;
}
ok=ret->data;
*head=list_remove_tail(*head);
*head=list_insert_head(*head,ok);
    return ok;
}

int lru(struct Node **head)
{
    int ok;
struct Node *ret =*head;
while(ret->next!=NULL){
ret=ret->next;
}
ok=ret->data;
*head=list_remove_tail(*head);
*head=list_insert_head(*head,ok);
    return ok;
}

int page_replacement(struct Node **head)
{
		int frame;
		int i;
		if(replacementPolicy == RANDOM)  frame = random();
		else if(replacementPolicy == FIFO)  frame = fifo(head);
		else if(replacementPolicy == LRU) frame = lru(head);

		return frame;
}

