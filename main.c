#include "alg_one.h"
#include "alg_two.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if (argc != 5) {
        printf("Incorrect arguments number, usage: program.x array_size num_threads bucket_per_thread debug\n");
        exit(EXIT_FAILURE);
    }

    long int array_size = atol(argv[1]);
    int num_threads = atoi(argv[2]);
    int num_buckets = atoi(argv[3]);
    int debug = atoi(argv[4]);

    //run(array_size, num_threads, num_buckets, debug);
    run_two(array_size, num_threads, num_buckets);

    // int start = 0;
    // int end = 99;
    // int length = 100;

    // int divider1 = 2;
    // int divider2 = 2;

    // float chunk1 = (float)length/divider1;
    // for(int i = 0; i < divider1; i++) {
    //     int start1 = i * chunk1;
    //     int end1 = (i+1) * chunk1;
    //     printf("chunk1 %f\n", chunk1);
    //     printf("end1 %d\n", end1);

    //     float chunk2 = (float)(end1 - start1) / divider2;
    //     printf("chunk2 %f\n", chunk2);
    //     for(int j = 0; j < divider2; j++) {
    //         printf("start: %d; end: %d\n", (int)(start1 + j * chunk2), (int)(start1 + (j+1) * chunk2 - 1));
    //     }
    // }

}