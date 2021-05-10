#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "random.h"

RandomStruct_t generate(long int array_size, int num_threads)
{
    int* array = malloc(array_size * sizeof(int));
    if (array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }
   
    omp_set_num_threads(num_threads);
    double t_start = omp_get_wtime();

    #pragma omp parallel
    {
        unsigned int seed = 21 + omp_get_thread_num();
        #pragma omp for schedule(guided, 6000)
        for (int i = 0; i < array_size; i++) {
            array[i] = rand_r(&seed)%MAX_RAND;
        }
    }
    double t_stop = omp_get_wtime();
    RandomStruct_t val = {.time = t_stop - t_start, .array = array, .array_size = array_size};
    return val;
}

void clean(RandomStruct_t obj) {
    free(obj.array);
}

void print_array(RandomStruct_t array, int array_size) {
    printf("array: [");
    for(int i = 0; i < array_size; i++) {
        printf("%d, ", array.array[i]);
    }
    printf("]\n");
}