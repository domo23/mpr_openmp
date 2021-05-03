#include "bucket.h"
#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"

Bucket_t BUCKET_create(int size, int min, int max) {
    int* array = malloc(size * sizeof(int));
    if (array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }
    Bucket_t bucket = {.array = array, .current_size = 0, .max = max, .max_size = size, .min = min};
    return bucket;
}

void BUCKET_clean(Bucket_t* bucket) {
    free(bucket->array);
}

void BUCKET_push(Bucket_t* bucket, int value) {
    if(bucket->current_size == bucket->max_size) {
        perror("Error with bucket overflow");
    }
    bucket->array[bucket->current_size] = value;
    bucket->current_size++;
}

void BUCKET_sort(Bucket_t* bucket) {
    quickSort(bucket->array, 0, bucket->current_size-1);
}

void BUCKET_print(Bucket_t* bucket) {
    printf("array: [");
    for(int i = 0; i < bucket->current_size; i++) {
        printf("%d, ", bucket->array[i]);
    }
    printf("]\nmin: %d\nmax: %d\nmax_size: %ld\ncurrent_size: %d\n", bucket->min, bucket->max, bucket->max_size, bucket->current_size);
}

Bucket_t BUCKET_concat(Bucket_t* bucket_array, int len, long max_size) {
    Bucket_t bucket = BUCKET_create(max_size, 0, 0);
    for(int i = 0; i < len; i++) {
        if(bucket_array[i].min < bucket.min) bucket.min = bucket_array[i].min;
        if(bucket_array[i].max > bucket.max) bucket.max = bucket_array[i].max;

        for(int j = 0; j < bucket_array[i].current_size; j++) {
            BUCKET_push(&bucket, bucket_array[i].array[j]);
        } 
    }

    return bucket;
}