struct node{
    int value;
    struct node *next;
};

struct queue{
    int size;
    struct node * head;
    struct node * tail;
};

// queue library
void init_q(struct queue *);
//void initm_q(struct queue *);
void add_q(struct queue *, int);
int empty_q(struct queue *);
int pop_q(struct queue *);
