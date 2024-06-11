#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Usage: %s <number_of_cells> <iterations>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int total_cars = 0;

    int* road = (int*)malloc((N+2) * sizeof(int));
    if (!road) return 1;  // Check if memory allocation failed

    srand(time(NULL));

    // Initialize road
    for (int i = 1; i <= N; i++) {
        road[i] = rand() % 2;
        total_cars += road[i];
    }

    road[0] = road[N];
    road[N+1] = road[1];

    int global_moves = 0;
    clock_t start_time = clock();

    // Simulation loop
    for (int iter = 0; iter < iterations; iter++) {
        int* new_road = (int*)malloc((N+2) * sizeof(int));
        int local_moves = 0;

        for (int i = 1; i <= N; i++) {
            if (road[i] == 1 && road[i+1] == 0) {
                new_road[i] = 0;
                new_road[i+1] = 1;
                local_moves++;
                i++; // skip next cell since car moved there
            } else {
                new_road[i] = road[i];
            }
        }

        free(road);
        road = new_road;
        road[0] = road[N];
        road[N+1] = road[1];
        global_moves += local_moves;
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double average_velocity = (double)global_moves / (iterations * total_cars);

    printf("%d, %f, %f\n", global_moves, elapsed_time, average_velocity);

    free(road);
    return 0;
}
