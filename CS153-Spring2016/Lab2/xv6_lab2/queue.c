#include "queue.h"
#include "types.h"
#include "user.h"

void init_q(struct queue *q){
    q->size = 0;
    q->head = 0;
    q->tail = 0;
}

void add_q(struct queue *q, int v){
    struct node * n = malloc(sizeof(struct node));
    n->next = 0;
    n->value = v;
    if(q->head == 0){
        q->head = n;
    }else{
        q->tail->next = n;
    }
    q->tail = n;
    q->size++;
}

int empty_q(struct queue *q){
    if(q->size == 0)
        return 1;
    else
        return 0;
}
int pop_q(struct queue *q){
    int val;
    struct node *destroy;
    if(!empty_q(q)){
       val = q->head->value;
       destroy = q->head;
       q->head = q->head->next;
       free(destroy);
       q->size--;
       if(q->size == 0){
            q->head = 0;
            q->tail = 0;
       }
       return val;
    }
    return -1;
}
