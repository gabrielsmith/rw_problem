typedef struct queue
{
  int id;
  struct node* first;
  struct node* last;
}queue_t;

void queue (queue_t*, int);
int dequeue (queue_t*);
