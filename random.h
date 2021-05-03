typedef struct RandomStruct {
   double   time;
   int*     array;
   long int array_size;
} RandomStruct_t;

RandomStruct_t generate(long int array_size, int num_threads);
void clean(RandomStruct_t obj);