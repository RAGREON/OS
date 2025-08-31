#include <stdio.h>
#include <stdlib.h>
#include "./required.h"

void simulate_round_robin(PCB** process_list, size_t count, int slice_time) {
  int completed = 0;
  int time = 0;

  int ready_queue[count] = { -1 };
  int ready_count = 0;
  int in_queue = 0;

  while (completed < count) {
    if (ready_count < count) {
      for (int i = 0; i < count; i++) {
        if (process_list[i]->remaining_time > 0 && process_list[i]->arrival_time >= time && ready_queue[i] != i) {
          ready_queue[i] = i;
          ready_count++;
          in_queue++;
        }
      }
    }

    for (int i = 0; i < count; i++) {
      if (ready_queue[i] == -1) continue;

      if (process_list[i]->remaining_time < slice_time)  {
        time += process_list[i]->remaining_time;
        process_list[i]->remaining_time = 0;
      }
      else {
        time += slice_time;
        process_list[i]->remaining_time -= slice_time;
      } 

      if (process_list[i]->remaining_time == 0) {
        completed++;
        in_queue--;
        ready_queue[i] = -1;
        process_list[i]->completion_time = time;
        process_list[i]->turnaround_time = time - process_list[i]->arrival_time;
        process_list[i]->waiting_time = process_list[i]->turnaround_time - process_list[i]->burst_time;
      }

      printf("p [%d] : %d\n", process_list[i]->process_id, time);

       if (ready_count < count) {
        for (int i = 0; i < count; i++) {
          if (process_list[i]->remaining_time > 0 && process_list[i]->arrival_time >= time && ready_queue[i] != i) {
            ready_queue[i] = i;
            ready_count++;
            in_queue++;
          }
        }
      }
    }

    if (in_queue < 0) time++;
  }
}

int main(int argc, char** argv) {
  srand(time(NULL));

  int count = atoi(argv[1]);
  int max_arrival = atoi(argv[2]);
  int max_burst = atoi(argv[3]);

  PCB** list = generate_list(count, max_arrival, max_burst);

  print_table(list, count);

  simulate_round_robin(list, count, 2);

  print_table(list, count);

  return 0;
}