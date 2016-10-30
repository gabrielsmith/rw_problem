typedef struct queue
{
  struct node* first;
  struct node* last;
}queue_t;

void queue (queue_t*, int);
int dequeue (queue_t*);
