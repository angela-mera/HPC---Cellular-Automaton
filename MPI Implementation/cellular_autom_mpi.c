#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(argc < 3) {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <number_of_cells> <iterations>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int local_N = N / size;
    int* local_road = (int*)malloc((local_N+2) * sizeof(int));
    if (!local_road) return 1;

    srand(time(NULL) + rank);

    // Initialize local road
    int total_cars = 0;
    for (int i = 1; i <= local_N; i++) {
        local_road[i] = rand() % 2;
        total_cars += local_road[i];
    }

    local_road[0] = local_road[local_N];
    local_road[local_N+1] = local_road[1];
    int global_moves = 0;
    double start_time = MPI_Wtime();

    // Simulation loop
    for (int iter = 0; iter < iterations; iter++) {
        int* new_local_road = (int*)malloc((local_N+2) * sizeof(int));
        int local_moves = 0;

        // Exchange boundaries
        int left_neighbor = (rank == 0) ? size - 1 : rank - 1;
        int right_neighbor = (rank == size - 1) ? 0 : rank + 1;

        MPI_Sendrecv(&local_road[local_N], 1, MPI_INT, right_neighbor, 0,
                     &local_road[0], 1, MPI_INT, left_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&local_road[1], 1, MPI_INT, left_neighbor, 0,
                     &local_road[local_N+1], 1, MPI_INT, right_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Apply rules
        for (int i = 1; i <= local_N; i++) {
            if (local_road[i] == 1 && local_road[i+1] == 0) {
                new_local_road[i] = 0;
                new_local_road[i+1] = 1;
                local_moves++;
                i++; // skip next cell since car moved there
            } else {
                new_local_road[i] = local_road[i];
            }
        }

        // Update road for next iteration
        for (int i = 0; i <= local_N + 1; i++) {
            local_road[i] = new_local_road[i];
        }
        free(new_local_road);  // Liberar la memoria después de copiar los datos

        // Reduce total moves to root process to calculate global moves
        int total_moves;
        MPI_Allreduce(&local_moves, &total_moves, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        if (rank == 0) {
            global_moves += total_moves;
        }
    }

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    // Calculate average velocity on the root process
    double average_velocity = 0;
    if (rank == 0) {
        average_velocity = (double)global_moves / (iterations * total_cars);
        printf("%d, %f, %f\n", global_moves, elapsed_time, average_velocity);
    }

    free(local_road);
    MPI_Finalize();
    return 0;
}