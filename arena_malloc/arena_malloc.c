#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// glibc uses 2*NUM_CORE on 32bit, and
// 8*NUM_CORE on 64bit.
#ifdef _LP64
#define CORE_MULTIPLICATOR 8
#else
#define CORE_MULTIPLICATOR 2
#endif

static
void perform_allocations(int count, int num) {
  printf("Malloc (%d) for thread: %d\n", count, num);
  void* payload;
  payload = malloc(sizeof(int) * 10); 
  if (payload == NULL) {
    return;
  }
  free(payload);
}

static
void* do_it(void* arg) {
  int num = *(int*)arg;
  int i;
  for (i = 0; i < 100; i++) {
    perform_allocations(i, num);
    usleep(1000 * 10);
  }
  return NULL;
}

static
void start_threads_and_run(int num, int num_procs) {
  printf("Spawning a total of %d (> (%d * %d)) threads\n", num, CORE_MULTIPLICATOR, num_procs);
  pthread_t _threads[num];
  int nums[num];
  int i, retval;
  for (i = 0; i < num; i++) {
    nums[i] = i;
    _threads[i] = 0;
    retval = pthread_create(&(_threads[i]), NULL, &do_it, &(nums[i]));
    if (retval != 0) {
      _threads[i] = 0;
    }
  }
  for (i = 0; i < num; i++) {
    if (_threads[i] != 0) {
      pthread_join(_threads[i], NULL);
    }
  }
  printf("Waited for all threads. Exiting now\n");
}

int main(int argc, char** args) {
  int num_conf_procs = get_nprocs_conf();
  int num_procs_tot = get_nprocs();
  printf("This system has %d processors configured and "
        "%d processors available.\n",
        num_conf_procs, num_procs_tot);
  int num_procs = num_procs_tot;
  if (num_conf_procs < num_procs_tot) {
    num_procs = num_conf_procs;
  }
  start_threads_and_run(num_procs * (CORE_MULTIPLICATOR + 1), num_procs);
  return 0;
}
