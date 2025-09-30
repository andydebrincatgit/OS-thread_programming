
#define MAXTHREADS 10000
#define MAXSLEEPTIME 100
#include "tid_map.h"
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int str_to_int(char *str);
void *thread_function(void *thread_arg);

// part a : The first argument will be the number of threads, then the min sleep time as the second and
// max as the third. They will be stored in the argv[] array starting from argv[1].

//Note that he input format in the command line should be : number_of_threads min_sleep max_sleep

//Also note that a function called str_to_int was created to convert the strings in the command line arguments to integers

int main(int argc, char *argv[]) {

  // Set the default values given in Task 2

  int number_of_threads = 100;
  int min_sleep_time = 1;
  int max_sleep_time = 10;

  // simple error checking
  if (argc > 4) {
    printf("Too many arguments\n");
    return 0;
  }

  // Depending on how many arguments were given the according values were
  // adjusted.
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


  // Since str_to_int returns -1 if the string is not a number, the inputs are
  // checked for validity here.
  if (min_sleep_time == -1 || max_sleep_time == -1 || number_of_threads == -1) {
    printf("Invalid input\n");
    return 0;
  }

  // If the min sleep time is greater than the max sleep time, the program
  // terminates.
  if (min_sleep_time >= max_sleep_time || max_sleep_time>=MAXSLEEPTIME) {
    printf("Invalid min or max sleep time\n");
    return 0;
  }

  // If the number of threads is too large (10000) or negative, the program will
  // terminate.
  if (number_of_threads < 1 || number_of_threads > MAXTHREADS) {
    printf("Invalid number of threads\n");
    return 0;
  }

  printf("Number of threads: %d\nMin sleep time : %d\nMax sleep time : %d\n",
         number_of_threads, min_sleep_time, max_sleep_time);

  // end of part a.

  // part b :

  // The allocate map function is called using the header file containg task1.c
  // called tid_map.h. Then it is checked if it is successful.
  if (allocate_map() == -1) {
    printf("Error allocating memory to tid_map\n");
    return 0;
  }

  // part e :

  // The necessary kernel_id variables that will be passed as arguments to
  // pthread_create are created on heap.
  pthread_t *kernel_thread_id_list =
      (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));

  if (kernel_thread_id_list == NULL) {
    printf("Error allocating memory\n");
  }

  // In order to pass the max and min sleep times as arguments, they are stored
  // in an integer array and typecast to a void pointer.
  int thread_args[2] = {min_sleep_time, max_sleep_time};
  void *thread_arg = (void *)thread_args;

  // The requested amount of threads are created using pthread_create.
  for (int i = 0; i < number_of_threads; i++) {
    pthread_create(&kernel_thread_id_list[i], NULL, &thread_function,
                   thread_arg);
  }

  // The threads are then joined and the result disregarded and stored in some
  // dummy variable.
  for (int i = 0; i < number_of_threads; i++) {
    void *result;
    pthread_join(kernel_thread_id_list[i], &result);
  }

  // All allocated memory is freed and a message is printed if the program is
  // successful.
  free(tidMap_ptr->tidList);
  free(tidMap_ptr);
  free(kernel_thread_id_list);

  printf("Successful\n");
}

// part c and part d:

void *thread_function(void *arg) {

  // The tid is allocated using the allocate tid function.
  int tid = allocate_tid();

  // If a tid is not available, the thread will terminate immediately.
  if (tid == -1) {
    printf("Error allocating tid\n");
    pthread_exit(NULL);
  }
  

  // The parameter passed will be a void pointer that points to two integer
  // values representing the min and max sleep times. The pointer is type casted
  // and derefernced to get the two integer values.
  int *args = (int *)arg;
  int min = args[0];
  int max = args[1];


  // The random value function is seeded using the thread id assigned by the
  // kernel in order to acheive a somewhat random result, that is more random than not seeding at all.
  srand(pthread_self());

  // Since the random function gives a value between 0 and RAND_MAX, the random
  // number is divided modulo the distance between min and max and the min sleep
  // time is added as a floor. This will acheive a random number in the given
  // range.
  int rand_int = rand() % (max - min + 1) + min;

  // The thread then sleeps for the random number of seconds.
  sleep(rand_int);



  // Finally the tid is released, and the thread terminates.
  release_tid(tid);
  pthread_exit(NULL);
}


// The str_to_int() function is a simple function that takes a string and
// converts it to an integer, or outputs -1 if the string is not an integer.
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

// Note that on repl.it the program will run only until 955 threads as an input
// for number of threads, probably due to memory resources. (Error message :
// Segmentation fault (core dumped))
