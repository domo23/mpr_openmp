typedef struct Bucket {
    int* array;
    int  min;
    int  max;
    long max_size;
    int  current_size;
} Bucket_t;

Bucket_t BUCKET_create(int size, int min, int max);
void BUCKET_clean(Bucket_t* bucket);
void BUCKET_push(Bucket_t* bucket, int value);
void BUCKET_sort(Bucket_t* bucket);
void BUCKET_print(Bucket_t* bucket);
Bucket_t BUCKET_concat(Bucket_t* bucket_array, int len, long max_size);