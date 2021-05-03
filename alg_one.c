#include "random.h"
#include "quicksort.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "bucket.h"

void print_array(RandomStruct_t array, int array_size) {
    printf("array: [");
    for(int i = 0; i < array_size; i++) {
        printf("%d, ", array.array[i]);
    }
    printf("]\n");
}

int run(long array_size, int num_threads) {
    RandomStruct_t array = generate(array_size, num_threads);

    omp_set_num_threads(num_threads);
    double t_start = omp_get_wtime();

    int chunk_index = array_size / num_threads;
    int chunk_value = RAND_MAX / num_threads;

    Bucket_t** bucket_array = malloc(num_threads * sizeof(Bucket_t*));
    if (bucket_array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    } 

    #pragma omp parallel shared(array)
    {
        int thread = omp_get_thread_num();
        unsigned int start_index = thread * chunk_index;
        unsigned int start_value = thread * chunk_value;
        unsigned int end_value = (thread + 1) * chunk_value -1;
        
        // Create bucket for thread and store it in global array
        Bucket_t bucket = BUCKET_create(chunk_index*3, start_value, end_value);  
        bucket_array[thread] = &bucket;

        // fill buckets in every thread
        for (int i = 0; i < array_size; i++) {
            int index = (start_index + i)%array_size;
            int val = array.array[index];
            if(val >= bucket.min && val <= bucket.max) BUCKET_push(&bucket, val);
        }

        // sort
        BUCKET_sort(&bucket);

        #pragma omp barrier
        // get start value of base array, based on size of other buckets
        // bucket filling have to be done before this step
        int start = 0;
        for(int i = 0; i < thread; i++) {
            //printf("Thread %d, size %d\n", i, bucket_array[i]->current_size);
            start+=bucket_array[i]->current_size;
        }

        // printf("Thread %d, start %d\n", thread, start);
        // printf("Bucket thread: %d\n", thread);
        // BUCKET_print(&bucket);
        // printf("\n");

        //fill based array with sorted values
        for(int i = 0; i < bucket.current_size; i++) {
            array.array[start+i] = bucket.array[i];
            printf("Thread %d, Index %d, value %d\n",thread, i+start, bucket.array[i]);
        }

        #pragma omp barrier
        BUCKET_clean(&bucket);
    }
    double t_stop = omp_get_wtime();

    print_array(array, array_size);

    clean(array);
    free(bucket_array);
}