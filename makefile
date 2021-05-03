ARRAY_SIZE := 10
THREADS := 4

all: build

build:
	gcc -o run.o main.c alg_one.c random.c bucket.c quicksort.c -fopenmp

run:
	./run.o ${ARRAY_SIZE} ${THREADS}

clean:
	rm -rf *.o
	rm -f results.txt