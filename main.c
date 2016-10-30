#include <stdio.h>                      // standard input/output
#include <stdlib.h>                     // standard library
#include <pthread.h>                    // POSIX threads
#include <semaphore.h>                  // POSIX semaphores
#include <string.h>                     // string manipulation



int data = 0;                           // resource to be written to/read from
int readers = 0;                        // number of threads reading the resource

sem_t res_access;                       // access to the resource (var data)
sem_t queue;                            // queue of requests
sem_t read_access;                      // access to readers counter var


void write_op ()
{
  data++;
}


void read_op ()
{
    printf("Data: %d\n", data);
}

void* writer (void *arg)
{
  printf("writer of id %d\n", *(int*)arg);

  sem_wait (&queue);                    // enters queue
  sem_wait (&res_access);               // requests access to the resorce
  sem_post (&queue);                    // leaves queue

  write_op ();                          // executes write operation
  
  sem_post (&res_access);               // frees the resource to the next in queue
}

void* reader (void *arg)
{
  printf("reader of id %d\n", *(int*)arg);

  sem_wait (&queue);                    // enters queue
  sem_wait (&read_access);              // requests access to readers counter
  
  if (readers == 0) sem_wait (&res_access); // requests access to the readers
  readers++;
  
  sem_post (&queue);                    // leaves queue
  sem_post (&read_access);              // frees the readers var to the next reader

  read_op ();                           // executes read operation

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

  printf("Semaphores initialized\n"); 
  
  char* rwqueue;
  if (argc > 1) rwqueue  = argv[1];
  else exit (1);
  
  int len = strlen (rwqueue);

  pthread_t threads[len];
  
  int i;
  for (i = 0; i < len; i++)
  {
    if (rwqueue[i] == 'R')
    {
      printf ("Created reader of id %d\n", i);
      pthread_create (&threads[i], NULL, &reader, (void*)&i);
    }

    else if (rwqueue[i] == 'W')
    {
      printf("Created writer of id %d\n", i);
      pthread_create (&threads[i], NULL, &writer, (void*)&i);
    }
    else exit (1);
  }

  return 0;
}
