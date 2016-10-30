#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int data;                               // resource to be written to/read from
int readers;                            // number of threads reading the resource

sem_t res_access;                       // access to the resource (var data)
sem_t queue;                            // queue of requests
sem_t read_access;                      // access to reader count var

void writer (int value)
{
  sem_wait (&queue);
  sem_wait (&res_access); 
  sem_post (&queue); 

  write(value);
  
  sem_post (&res_access);
}

int reader ()
{
  sem_wait (&queue);
  sem_wait (&read_access);
  
  if (readers == 0) sem_wait (&res_access);
  readers++;
  
  sem_post (&queue);
  sem_post (&read_access);

  read();

  // TODO: finish reader code
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
