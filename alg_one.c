#include "random.h"
#include "quicksort.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "bucket.h"
#include "measure.h"

int run(long array_size, int num_threads, int bucket_per_thread, int debug) {
    Measurements_t* measurements = MEASURE_createMeasurements(num_threads);
    double t_start = omp_get_wtime();
    RandomStruct_t array = generate(array_size, num_threads);
    printf("Array generate time: %f s\n", array.time);
    if(debug) print_array(array, array_size);

    omp_set_num_threads(num_threads);

    int chunk_index = array_size / num_threads;
    float chunk_value = (float)MAX_RAND / num_threads;

    BucketArray_t ** global_bucket_array = malloc(num_threads * sizeof(BucketArray_t*));
    if (global_bucket_array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    } 

    #pragma omp parallel shared(array)
    {
        int thread = omp_get_thread_num();
        unsigned int start_index = thread * chunk_index;
        unsigned int start_value = (float)thread * chunk_value;
        unsigned int end_value = (float)(thread + 1) * chunk_value;
        
        // Create bucket array for thread and assign it to global array
        BucketArray_t* local_bucket_array = BUCKET_createArray(chunk_index, start_value, end_value, bucket_per_thread);
        global_bucket_array[thread] = local_bucket_array;

        // fill buckets in every thread
        double t_start_fill = omp_get_wtime();
        for (int i = 0; i < array_size; i++) {
            int index = (start_index + i)%array_size;
            int val = array.array[index];
            BUCKET_arrayPushIf(local_bucket_array, val);
        }
        double t_end_fill = omp_get_wtime();
        printf("Thread %d bucket fill time %f s\n", thread, t_end_fill - t_start_fill);

        // It is crucial to for filling base array to know size of other buckets
        #pragma omp barrier

        // sort bucket array with quick sort
        double t_start_sort = omp_get_wtime();
        BUCKET_arraySort(local_bucket_array);
        double t_end_sort = omp_get_wtime();
        printf("Thread %d bucket sort time %f s\n", thread, t_end_sort - t_start_sort);

        // get start value of base array, based on size of other buckets
        // bucket filling have to be done before this step
        double t_start_replace = omp_get_wtime();
        int start = 0;
        for(int i = 0; i < thread; i++) {
            start+=global_bucket_array[i]->current_size;
        }

        // merge local array bucket to one bucket
        if(debug) BUCKET_arrayPrint(local_bucket_array);
        Bucket_t* merged_bucket = BUCKET_arrayMerge(local_bucket_array);

        // fill based array with sorted values
        for(int i = 0; i < merged_bucket->current_size; i++) {
            array.array[start+i] = merged_bucket->array[i];
        }
        double t_end_replace = omp_get_wtime();
        printf("Thread %d bucket replace time %f s\n", thread, t_end_replace - t_start_replace);
        
        // we can clean buckets only after all threads are done
        #pragma omp barrier
        measurements->thread_measurements[thread]->fill = t_end_fill - t_start_fill;
        measurements->thread_measurements[thread]->sort = t_end_sort - t_start_sort;
        measurements->thread_measurements[thread]->replace = t_end_replace - t_start_replace;
        BUCKET_clean(merged_bucket);
        BUCKET_cleanArray(local_bucket_array);
    }
    double t_stop = omp_get_wtime();
    printf("Overall time: %f s\n", t_stop - t_start);
    if(debug) print_array(array, array_size);

    measurements->array_size = array_size;
    measurements->bucket_per_thread = bucket_per_thread;
    measurements->generate_array = array.time;
    measurements->overall = t_stop - t_start;
    MEASURE_saveMeasurementsCSV(measurements, "test");

    MEASURE_cleanMeasurements(measurements);
    clean(array);
    free(global_bucket_array);
}