#include "bucket.h"
#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"

Bucket_t* BUCKET_create(int size, int min, int max) {
    int* array = malloc(size * sizeof(int));
    if (array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    Bucket_t* bucket_p = malloc(sizeof(Bucket_t));
    if (bucket_p == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    bucket_p->array = array;
    bucket_p->current_size = 0;
    bucket_p->max = max;
    bucket_p->min = min;
    bucket_p->max_size = size;
    
    return bucket_p;
}

void BUCKET_clean(Bucket_t* bucket) {
    free(bucket->array);
    free(bucket);
}

void BUCKET_push(Bucket_t* bucket, int value) {
    if(bucket->current_size == bucket->max_size) {
        perror("Error with bucket overflow");
    }
    bucket->array[bucket->current_size] = value;
    bucket->current_size++;
}

int BUCKET_pushIf(Bucket_t* bucket, int value) {
    if(value >= bucket->min && value <= bucket->max) {
        BUCKET_push(bucket, value);
        return 1;
    }
    return 0;
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

Bucket_t* BUCKET_concat(Bucket_t* bucket_array, int len, long max_size) {
    Bucket_t* bucket = BUCKET_create(max_size, 0, 0);
    for(int i = 0; i < len; i++) {
        if(bucket_array[i].min < bucket->min) bucket->min = bucket_array[i].min;
        if(bucket_array[i].max > bucket->max) bucket->max = bucket_array[i].max;

        for(int j = 0; j < bucket_array[i].current_size; j++) {
            BUCKET_push(bucket, bucket_array[i].array[j]);
        } 
    }

    return bucket;
}

BucketArray_t* BUCKET_createArray(int bucket_size, int min, int max, int bucket_amount) {
    Bucket_t** bucket_array = malloc(bucket_amount * sizeof(Bucket_t*));
    if (bucket_array == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    int chunk_index = (bucket_size / bucket_amount) * 3;
    float chunk_value = (float)(max - min) / bucket_amount;
    int local_min, local_max;
    for(int i = 0; i < bucket_amount; i++) {
        local_min = (int)(min + i * chunk_value);
        local_max = (int)(min + (i+1) * chunk_value - 1);

        Bucket_t* bucket = BUCKET_create(chunk_index, local_min, local_max);
        bucket_array[i] = bucket;
    }

    BucketArray_t* bucket_array_t = malloc(sizeof(BucketArray_t));
    if (bucket_array_t == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    bucket_array_t->bucket_array = bucket_array;
    bucket_array_t->bucket_count = bucket_amount;
    bucket_array_t->max = max;
    bucket_array_t->min = min;

    return bucket_array_t;
}

void BUCKET_cleanArray(BucketArray_t * bucket_array) {
    for(int i = 0; i < bucket_array->bucket_count; i++) {
        BUCKET_clean(bucket_array->bucket_array[i]);
    }
    free(bucket_array->bucket_array);
    free(bucket_array);
}

int BUCKET_arrayPushIf(BucketArray_t* bucket_array, int value) {
    if(value < bucket_array->min || value > bucket_array->max) return 0;
    for(int i = 0; i < bucket_array->bucket_count; i++) {
        Bucket_t* bucket_p = bucket_array->bucket_array[i];
        if(BUCKET_pushIf(bucket_p, value)) {
            bucket_array->current_size++;
            return 1;
        }
    }
}

void BUCKET_arraySort(BucketArray_t* bucket_array) {
    for(int i = 0; i < bucket_array->bucket_count; i++) {
        BUCKET_sort(bucket_array->bucket_array[i]);
    }
}

Bucket_t* BUCKET_arrayMerge(BucketArray_t* bucket_array) {
    Bucket_t* new_bucket = BUCKET_create(bucket_array->current_size, bucket_array->min, bucket_array->max);

    for(int i = 0; i < bucket_array->bucket_count; i++) {
        Bucket_t* bucket_p = bucket_array->bucket_array[i]; 
        for(int j = 0; j < bucket_p->current_size; j++) {
            BUCKET_push(new_bucket, bucket_p->array[j]);
        }
    }

    return new_bucket;
}

void BUCKET_arrayPrint(BucketArray_t* bucket_array) {
    for(int i = 0; i < bucket_array->bucket_count; i++) {
        BUCKET_print(bucket_array->bucket_array[i]);
    }
}