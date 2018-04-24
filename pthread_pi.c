/** Multithreading pi calculation example via pthreads in C 
    To compile (Unix-like system): gcc -o pthread_pi pthread_pi.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

long thread_count;
long long n;
static double sum = 0.0;
static pthread_mutex_t lock;

void* PI_calc(void* rank);

/* Report error and exit program */
void report_and_exit(const char* msg) {
    perror(msg);
    exit(-1);    /** failure **/
}

int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;

    if (argc != 3) {
        fprintf(stderr, "Usage: pthread_pi <number of threads> <n>\n");
        fprintf(stderr, "   n is the number of terms to sum\n");
        fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
        exit(0);
    }

    thread_count = atol(argv[1]);
    n = atoll(argv[2]);

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
    pthread_mutex_init(&lock, 0);

    for (thread = 0; thread < thread_count; thread++) {
        if(pthread_create(&thread_handles[thread], 0, 
            PI_calc, (void*)thread) < 0) {
                report_and_exit("pthread create failure.");
            }
    } 
 
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], 0);
    }

    sum = 4.0 * sum;
    printf("With n = %lld and %ld pthreads\n",n, thread_count);
    printf("    Our estimate of pi = %.15f\n", sum);
    printf("    Actual value of pi = %.15f\n", 4.0*atan(1.0));

    pthread_mutex_destroy(&lock);
    free(thread_handles);
    return 0;
}


void* PI_calc(void* rank) {
    long my_rank = (long) rank;
    double factor;
    long long i;
    long long my_n = n/thread_count;
    long long my_first_i = my_n*my_rank;
    long long my_last_i = my_first_i + my_n;
    double my_sum = 0.0;
 
    if (my_first_i % 2 == 0)
       factor = 1.0;
    else
       factor = -1.0;
 
    for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
       my_sum += factor/(2*i+1);
    }

    if (pthread_mutex_lock(&lock) == 0) {
        sum += my_sum;
        pthread_mutex_unlock(&lock);
    } 
    else {
        report_and_exit("pthread mutex lock failure.");
    }
 
    return NULL;
 }  