// Peterson's solution to race condition

#include <stdio.h>
#include <pthread.h>

#define PROCESSES 2

int turn, val = 0;
int interested[PROCESSES];

typedef struct {
  int process;
  int n;
} ThreadArgs;

void enterRegion(int process) {
  int other = 1 - process;
  interested[process] = 1;
  turn = process;
  
  while (turn == process && interested[other] == 1);
}

void leaveRegion(int process) {
  interested[process] = 0; 
}

void lockInit() {
  interested[0] = interested[1] = 0;
  turn = 0;
}

void* workThread(void* args) {
  ThreadArgs* t_args = (ThreadArgs*)args;
  int process = t_args->process;
  int n = t_args->n;

  enterRegion(process);

  for (int i = 0; i < n; i++) 
    val++;  

  leaveRegion(process);

  return NULL;
}

int main(int argc, char** argv) {
  pthread_t thread_1, thread_2;

  lockInit();

  ThreadArgs arg_1 = { 0, 1000000 }, arg_2 = { 1, 200000 };

  pthread_create(&thread_1, NULL, workThread, &arg_1);
  pthread_create(&thread_2, NULL, workThread, &arg_2);
  
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);

  printf("final value: %d\n", val);

  return 0;
}