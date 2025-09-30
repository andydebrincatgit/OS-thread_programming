#define MINTID 300
#define MAXTID 5000
#define MAXTHREADS 10000
#define MAXSLEEPTIME 100
#define MAXTRIES 50
#include "tid_map.h"
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// This file is an answer to part b) of task 3 and is an adjustment made to part
// a) of task 3.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int str_to_int(char *str);
void *thread_function(void *thread_arg);

int main(int argc, char *argv[]) {

  int number_of_threads = 100;
  int min_sleep_time = 1;
  int max_sleep_time = 10;

  if (argc > 4) {
    printf("Too many arguments\n");
    return 0;
  }

  else if (argc == 4) {
    number_of_threads = str_to_int(argv[1]);
    min_sleep_time = str_to_int(argv[2]);
    max_sleep_time = str_to_int(argv[3]);
  } else if (argc == 3) {
    number_of_threads = str_to_int(argv[1]);
    min_sleep_time = str_to_int(argv[2]);
  } else if (argc == 2) {
    number_of_threads = str_to_int(argv[1]);
  }

  if (min_sleep_time == -1 || max_sleep_time == -1 || number_of_threads == -1) {
    printf("Invalid input\n");
    return 0;
  }

  if (min_sleep_time >= max_sleep_time || max_sleep_time>=MAXSLEEPTIME) {
    printf("Invalid min or max sleep time\n");
    return 0;
  }

  if (number_of_threads < 1 || number_of_threads > MAXTHREADS) {
    printf("Invalid number of threads\n");
    return 0;
  }

  printf("Number of threads: %d\nMin sleep time : %d\nMax sleep time : %d\n",
         number_of_threads, min_sleep_time, max_sleep_time);

  if (allocate_map() == -1) {
    printf("Error allocating memory to tid_map\n");
    return 0;
  }

  pthread_t *kernel_thread_id_list =
      (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));

  if (kernel_thread_id_list == NULL) {
    printf("Error allocating memory\n");
  }

  int thread_args[2] = {min_sleep_time, max_sleep_time};
  void *thread_arg = (void *)thread_args;

  for (int i = 0; i < number_of_threads; i++) {
    pthread_create(&kernel_thread_id_list[i], NULL, &thread_function,
                   thread_arg);
  }

  for (int i = 0; i < number_of_threads; i++) {
    void *result;
    pthread_join(kernel_thread_id_list[i], &result);
  }

  free(tidMap_ptr->tidList);
  free(tidMap_ptr);
  free(kernel_thread_id_list);

  pthread_mutex_destroy(&mutex);

  printf("Successful\n");
}

void *thread_function(void *arg) {

  // Using try_lock, multiple attempts to get a mutex lock are made until a
  // maximun number of tries defined by a constant. If try_lock does not succeed
  // the thread sleeps for a short period of time specified by the kernel_id modulo
  // (max - min ) so that the sleep time is not too large (becasue even in microseconds
  // the kernel id is very large)and then tries again, until the max tries. This sleeping
  // time increases linearly as the number of tries
  // increases. If try_lock does not succeed after the max tries, the thread is
  // aborted.

  int tries = 0;
  pthread_t kernel_thread_id = pthread_self();
  int *args = (int *)arg;
  int min = args[0];
  int max = args[1];

  while (tries < MAXTRIES) {
    if (pthread_mutex_trylock(&mutex) == 0) {
      break;
    }
    usleep((kernel_thread_id%((max - min + 1 )*(tries+1)))+1);
    tries++;
  }

  // In order to avoid deadlocks and infinte loops, after a maximun number of
  // failures using trylock, the programa exits.
  if (tries == MAXTRIES) {
    printf("No mutex lock could be acquired\n");
    exit(1);
  }

  int tid = allocate_tid();

  pthread_mutex_unlock(&mutex);


  if (tid == -1) {
    printf("Error allocating tid\n");
    pthread_exit(NULL);
  }

  srand(pthread_self());
  int rand_int = rand() % (max - min + 1) + min;

  sleep(rand_int);


  // The same exact method for acquiring a lock is used again for releasing the
  // tid.

  tries = 0;
  while (tries < MAXTRIES) {
    if (pthread_mutex_trylock(&mutex) == 0) {
      break;
    }
    usleep((kernel_thread_id%((max - min + 1 )*(tries+1)))+1);
    tries++;
  }

  if (tries == MAXTRIES) {
    printf("No mutex lock could be acquired\n");
    exit(1);
  }

  release_tid(tid);


  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int str_to_int(char *str) {
  int ret = 0;
  int i = 0;
  while (str[i] != '\0') {
    if (isdigit(str[i])) {
      ret = ret * 10 + (str[i] - '0');
    } else {
      return -1;
    }
    i++;
  }
  return ret;
}
