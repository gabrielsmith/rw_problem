typedef struct node
{
  int id;
  struct node* next;
}node_t;

void node_create (struct node*, int, struct node*);
int node_get_value (struct node*);
void free_node (struct node*);
