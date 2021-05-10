ARRAY_SIZE := 200
THREADS := 3
BUCKETS := 3
DEBUG := 0

all: build

build:
	gcc -o run.o main.c alg_one.c random.c bucket.c quicksort.c measure.c -fopenmp

run:
	./run.o ${ARRAY_SIZE} ${THREADS} ${BUCKETS} ${DEBUG}

clean:
	rm -rf *.o
	rm -f results.txt