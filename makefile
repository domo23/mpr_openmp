ARRAY_SIZE := 200
THREADS := 3
BUCKETS := 3
DEBUG := 0

all: build

build:
	gcc -g -o run.o main.c alg_one.c alg_two.c random.c bucket.c quicksort.c measure.c -fopenmp

run:
	./run.o ${ARRAY_SIZE} ${THREADS} ${BUCKETS} ${DEBUG}

clean:
	rm -rf *.o
	rm -f results.txt