#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "random.h"
#include "quicksort.h"
#include "bucket.h"
#include "alg_two.h"

void run_two(long int array_size, int num_threads, int bucket_per_thread)
{
    double global_start = omp_get_wtime();
    RandomStruct_t array = generate(array_size, num_threads);

    omp_set_num_threads(num_threads);

    int chunk_index = array_size / num_threads;
    int chunk_value = MAX_RAND / num_threads;

    // create buckets
    int buckets_number = bucket_per_thread * num_threads;
    BucketArray_t *bucket_array = BUCKET_createArray(array_size, 0, MAX_RAND, buckets_number);

    int elements_per_thread = array_size / num_threads;
    double bucket_division_start = 0.0, bucket_division_stop = 0.0;
    double sort_start = 0.0, sort_stop = 0.0;
    double fill_array_start = 0.0, fill_array_stop = 0.0;

    #pragma omp parallel shared(array, bucket_array)
    {
        int thread = omp_get_thread_num();
        unsigned int start_index = thread * elements_per_thread;
        unsigned int end_index = (thread + 1) * elements_per_thread;

        if (thread == 0) bucket_division_start = omp_get_wtime();

        // fill buckets in every thread
        for (int i = start_index; i < end_index; i++) {
            int val = array.array[i];

            // find bucket
            int bucket_index = -1;
            for (int j = 0; (j < buckets_number) && (bucket_index == -1); j++) {
                if (bucket_array->bucket_array[j]->min <= val && bucket_array->bucket_array[j]->max >= val)
                    bucket_index = j;
            }

            #pragma omp critical
            BUCKET_push(bucket_array->bucket_array[bucket_index], val);
        }

        // we need to wait until all buckets are filled before sorting
        #pragma omp barrier
        if (thread == 0) bucket_division_stop = sort_start = omp_get_wtime();

        // sort
        unsigned int sort_start_index = thread * bucket_per_thread;
        unsigned int sort_end_index = (thread + 1) * bucket_per_thread;
        for (int i = sort_start_index; i < sort_end_index; i++)
            BUCKET_sort(bucket_array->bucket_array[i]);

        #pragma omp barrier
        if (thread == 0) sort_stop = fill_array_start = omp_get_wtime();
        // get start value of base array, based on size of other buckets
        // bucket filling have to be done before this step
        int start = 0;
        for(int i = 0; i < sort_start_index; i++) {
            start += bucket_array->bucket_array[i]->current_size;
        }

        //fill based array with sorted values
        int shift = 0;
        for (int i = sort_start_index; i < sort_end_index; i++) {
            for(int j = 0; j < bucket_array->bucket_array[i]->current_size; j++, shift++)
                array.array[start + shift] = bucket_array->bucket_array[i]->array[j];
        }

        #pragma omp barrier
        if (thread == 0) fill_array_stop = omp_get_wtime();
        
        /*for (int i = sort_start_index; i < sort_end_index; i++)
            BUCKET_clean(bucket_array->bucket_array[i]);*/
    }
    BUCKET_cleanArray(bucket_array);
    double global_stop = omp_get_wtime();

    printf("Time report:\n");
    printf("\tRandom array generation:\t\t%f s\n", array.time);
    printf("\tDivision into buckets:\t\t\t%f s\n", bucket_division_stop - bucket_division_start);
    printf("\tSorting buckets:\t\t\t%f s\n", sort_stop - sort_start);
    printf("\tFilling array with sorted values:\t%f s\n", fill_array_stop - fill_array_start);
    printf("\tTotal:\t\t\t\t\t%f s\n\n", global_stop - global_start);

    clean(array);
}