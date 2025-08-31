#pragma once

#include <time.h>
#include <stdlib.h>

typedef struct {
  int process_id;
  int arrival_time;
  int burst_time;
  int remaining_time;
  int completion_time;
  int turnaround_time;
  int waiting_time;
} PCB;

PCB* create_process(int id, int arrival_time, int burst_time) {
  PCB* process = (PCB*)malloc(sizeof(PCB));

  process->process_id = id;
  process->arrival_time = arrival_time;
  process->burst_time = burst_time;
  process->remaining_time = burst_time;

  process->completion_time = 0;
  process->turnaround_time = 0;
  process->waiting_time = 0;

  return process;
}

typedef struct Node {
  PCB* process;
  struct Node* next;
} Node;

typedef struct {
  PCB* process;

  Node* front;
  Node* rear;
} Queue;

Node* create_node(PCB* process);

Queue* create_queue();
int is_empty(Queue* queue);
void enqueue(Queue* queue, PCB* process);
void dequeue(Queue* queue, int process_id);
void traverse_queue(Queue* queue, int slice_time);

Node* create_node(PCB* process) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->process = process;
  node->next = NULL;
  return node;
}

Queue* create_queue() {
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  queue->front = queue->rear = NULL;
  return queue;
}

int is_empty(Queue* queue) {
  return (queue->front == NULL);
}

void enqueue(Queue* queue, PCB* process) {
  Node* newNode = create_node(process);
  if (queue->front == NULL) {
    queue->front = queue->rear = newNode;
    return;
  }
  queue->rear->next = newNode;
  queue->rear = newNode;
}

void dequeue(Queue* queue, int process_id) {
  if (is_empty(queue)) return; 

  Node* temp = queue->front;
  Node* prev = NULL;

  if (temp != NULL && temp->process->process_id == process_id) {
    queue->front = queue->front->next;
    if (queue->front == NULL) {
      queue->rear = NULL;
    }
    free(temp);
    return;
  }

  while (temp != NULL && temp->process->process_id != process_id) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) {
    return;
  }

  prev->next = temp->next;
  if (temp == queue->rear) {
    queue->rear = prev;
  }

  free(temp);
}

void traverse_queue(Queue* queue, int slice_time) {
  Node* temp = queue->front;

  while (temp != NULL) {
    int dequeue_id = -1;

    if (temp->process->remaining_time < slice_time) {
      temp->process->remaining_time = 0;
      dequeue_id = temp->process->process_id;
    }
    else {
      temp->process->remaining_time -= slice_time;
    }

    temp = temp->next;

    if (dequeue_id != -1) dequeue(queue, dequeue_id);
  }
}

PCB** generate_list(size_t count, int max_arrival, int max_burst) {
  PCB** list = (PCB**)malloc(sizeof(PCB*) * count);

  for (int i = 0; i < count; i++) {
    int arrival_time = rand() % max_arrival;
    int burst_time = rand() % max_burst + 1;

    PCB* process = create_process(i + 1, arrival_time, burst_time);

    list[i] = process;
  }

  return list;
}

void print_table(PCB** processes, int n) {
    int table_width = 110;
    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n");

    printf("| %-10s | %-12s | %-10s | %-15s | %-15s | %-15s | %-12s |\n",
           "ProcessID", "ArrivalTime", "BurstTime", "RemainingTime",
           "CompletionTime", "TurnaroundTime", "WaitingTime");

    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("| %-10d | %-12d | %-10d | %-15d | %-15d | %-15d | %-12d |\n",
               processes[i]->process_id,
               processes[i]->arrival_time,
               processes[i]->burst_time,
               processes[i]->remaining_time,
               processes[i]->completion_time,
               processes[i]->turnaround_time,
               processes[i]->waiting_time);
    }

    for (int i = 0; i < table_width; i++) printf("-");
    printf("\n");
}
