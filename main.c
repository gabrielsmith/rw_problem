#include <stdio.h>                      // standard input/output
#include <stdlib.h>                     // standard library
#include <pthread.h>                    // POSIX threads
#include <semaphore.h>                  // POSIX semaphores
#include <string.h>                     // string manipulation

int* queue_ids;                         // holds the ids of the threads in queue (DEBUGGING)
int last_pos = 0;                       // position of the last thread id in queue (DEBUGGING)

int data = 0;                           // resource to be written to/read from
int readers = 0;                        // number of threads reading the resource

sem_t res_access;                       // access to the resource (var data)
sem_t queue;                            // queue of requests
sem_t read_access;                      // access to readers counter var
sem_t debug;                            // access to debugging variables

void write_op (int id)
{
  printf ("--Executing write operation of thread of id %d\n", id);
  data++;
}


void read_op (int id)
{
  printf ("--Reader of id %d reading data: %d\n", id, data);
}

void* writer (void *arg)
{
  sem_wait (&debug);
  queue_ids[last_pos++] = *(int*)arg;
  sem_wait (&queue);                    // enters queue
  sem_post (&debug);
  sem_wait (&res_access);               // requests access to the resorce
  sem_post (&queue);                    // leaves queue

  write_op (*(int*)arg);                // executes write operation
  
  sem_post (&res_access);               // frees the resource to the next in queue
}

void* reader (void *arg)
{
  sem_wait (&debug);
  queue_ids[last_pos++] = *(int*)arg;
  sem_wait (&queue);                    // enters queue
  sem_post (&debug);
  sem_wait (&read_access);              // requests access to readers counter
  
  if (readers == 0) sem_wait (&res_access); // requests access to the readers
  readers++;
  
  sem_post (&queue);                    // leaves queue
  sem_post (&read_access);              // frees the readers var to the next reader

  read_op (*(int*)arg);                 // executes read operation

  sem_wait (&read_access);              // requests access to the readers var again
  readers--;
  if (readers == 0) sem_post (&res_access); // frees resource when all readers have finished
  sem_post (&read_access);              // frees the readers var
}

int main (int argc, char** argv)
{
  printf("\nMain started\n");

  sem_init (&res_access, 0, 1);
  sem_init (&queue, 0, 1);
  sem_init (&read_access, 0, 1);
  sem_init (&debug, 0, 1);

  printf("Semaphores initialized\n"); 
  
  char* rwqueue;
  if (argc > 1) rwqueue  = argv[1];
  else exit (1);
  
  int len = strlen (rwqueue);

  int q_ids[len];
  queue_ids = q_ids;

  pthread_t threads[len];
  int id[len];

  int i;
  for (i = 0; i < len; i++)
  {
    id[i] = i;
    if (rwqueue[i] == 'R')
    {
      //printf ("Created reader of id %d\n", i);
      pthread_create (&threads[i], NULL, &reader, (void*)&id[i]);
    }

    else if (rwqueue[i] == 'W')
    {
      //printf("Created writer of id %d\n", i);
      pthread_create (&threads[i], NULL, &writer, (void*)&id[i]);
    }
    else exit (1);
  }
  
  for (i = 0; i < len; i++)
  {
    pthread_join (threads[i], NULL);
  }

  printf ("Id queue: ");
  for (i = 0; i < len; i++)
  {
    printf ("%d ", queue_ids[i]);
  }
  printf("\n");

  return 0;
}
