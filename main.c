#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int data;                               // resource to be written to/read from
int readers;                            // number of threads reading the resource

sem_t res_access;                       // access to the resource (var data)
sem_t queue;                            // queue of requests
sem_t read_access;                      // access to readers counter var


void write ()
{
  data++;
}


void read ()
{
  printf("Data: %d", data);
}

void writer ()
{
  sem_wait (&queue);                    // enters queue
  sem_wait (&res_access);               // requests access to the resorce
  sem_post (&queue);                    // leaves queue

  write();                              // executes write operation
  
  sem_post (&res_access);               // frees the resource to the next in queue
}

void reader ()
{
  sem_wait (&queue);                    // enters queue
  sem_wait (&read_access);              // requests access to readers counter
  
  if (readers == 0) sem_wait (&res_access); // requests access to the readers
  readers++;
  
  sem_post (&queue);                    // leaves queue
  sem_post (&read_access);              // frees the readers var to the next reader

  read();                               // executes read operation

  sem_wait (&read_access);              // requests access to the readers var again
  readers--;
  if (readers == 0) sem_post (&res_access); // frees resource when all readers have finished
  sem_post (&read_access);              // frees the readers var
}

int main (int argc, char** argv)
{
  data = 0;
  readers = 0;

  sem_init (&res_access, 0, 1);
  sem_init (&queue, 0, 1);
  sem_init (&read_access, 0, 1);
  
  // TODO: interface to test code
  
  return 0;
}
