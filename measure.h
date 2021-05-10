typedef struct ThreadMeasurements {
    double fill;
    double sort;
    double replace;
} ThreadMeasurements_t;

typedef struct Measurements {
    double generate_array;
    double overall;
    long array_size; 
    int num_threads; 
    int bucket_per_thread;
    ThreadMeasurements_t** thread_measurements;
} Measurements_t;

ThreadMeasurements_t* MEASURE_createThreadMeasurements();
Measurements_t* MEASURE_createMeasurements(int num_threads);
void MEASURE_cleanThreadMeasurements(ThreadMeasurements_t* measurements);
void MEASURE_cleanMeasurements(Measurements_t* measurements);
void MEASURE_saveMeasurementsCSV(Measurements_t* measurements, char* filename);