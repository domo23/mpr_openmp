#include "measure.h"
#include <stdio.h>
#include <stdlib.h>

ThreadMeasurements_t* MEASURE_createThreadMeasurements() {
    ThreadMeasurements_t* measurements = malloc(sizeof(ThreadMeasurements_t));
    if (measurements == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }
    return measurements;
} 

Measurements_t* MEASURE_createMeasurements(int num_threads) {
    Measurements_t* measurements = malloc(sizeof(Measurements_t));
    if (measurements == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    ThreadMeasurements_t** thread_measurements = malloc(num_threads * sizeof(ThreadMeasurements_t*));
    if (thread_measurements == NULL) {
        perror("Error with memory allocation");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < num_threads; i++) {
        thread_measurements[i] = MEASURE_createThreadMeasurements();
    }

    measurements->thread_measurements = thread_measurements;
    measurements->num_threads = num_threads;
}

void MEASURE_cleanThreadMeasurements(ThreadMeasurements_t* measurements) {
    free(measurements);
}

void MEASURE_cleanMeasurements(Measurements_t* measurements) {
    for(int i = 0; i < measurements->num_threads; i++) {
        free(measurements->thread_measurements[i]);
    }
    free(measurements);
}

void MEASURE_saveMeasurementsCSV(Measurements_t* measurements, char* filename) {
    FILE *fptr = fopen("results.csv", "w");
    fprintf(fptr, "generate_array_time,overall_time,array_size,num_threads,bucket_per_thread,fill_time,sort_time,replace_time\n");
    for(int i = 0; i < measurements->num_threads; i++) {
        ThreadMeasurements_t* t_meas_p = measurements->thread_measurements[i];
        fprintf(fptr, "%f,%f,%ld,%d,%d,%f,%f,%f\n", measurements->generate_array, measurements->overall, measurements->array_size, 
                measurements->num_threads, measurements->bucket_per_thread, t_meas_p->fill, t_meas_p->sort, t_meas_p->replace);
    }
    fclose(fptr);
}