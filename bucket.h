typedef struct Bucket {
    int* array;
    int  min;
    int  max;
    long max_size;
    int  current_size;
} Bucket_t;

typedef struct BucketArray {
    Bucket_t ** bucket_array;
    int         min;
    int         max;
    int         bucket_count;
    int         current_size;
} BucketArray_t;

Bucket_t* BUCKET_create(int size, int min, int max);
void BUCKET_clean(Bucket_t* bucket);
void BUCKET_push(Bucket_t* bucket, int value);
int BUCKET_pushIf(Bucket_t* bucket, int value);
void BUCKET_sort(Bucket_t* bucket);
void BUCKET_print(Bucket_t* bucket);
Bucket_t* BUCKET_concat(Bucket_t* bucket_array, int len, long max_size);
BucketArray_t* BUCKET_createArray(int bucket_size, int min, int max, int bucket_amount);
void BUCKET_cleanArray(BucketArray_t * bucket_array);
int BUCKET_arrayPushIf(BucketArray_t* bucket_array, int value);
void BUCKET_arraySort(BucketArray_t* bucket_array);
void BUCKET_arrayPrint(BucketArray_t* bucket_array);
Bucket_t* BUCKET_arrayMerge(BucketArray_t* bucket_array);