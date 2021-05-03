#include "alg_one.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Incorrect arguments number, usage: program.x array_size num_threads\n");
        exit(EXIT_FAILURE);
    }

    long int array_size = atol(argv[1]);
    int num_threads = atoi(argv[2]);

    run(array_size, num_threads);
}