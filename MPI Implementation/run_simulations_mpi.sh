#!/bin/bash

# Compile the program
mpicc -o cellular_autom_mpi_exe cellular_autom_mpi.c
echo "Compilación del programa MPI completada."

# File to store the results
echo "Tipo, Tamaño, Iteraciones, Movimientos totales, Tiempo total, Velocidad promedio" > results_mpi.csv

# Simulation parameters
sizes=(100000 200000 300000 400000 500000)

# Execute simulations
if [ "$(command -v mpirun)" ]; then
    for i in {1..10}; do
        echo "Iniciando iteración $i de 10..."
        for N in "${sizes[@]}"; do
            mpi_output=$(mpirun -np 4 ./cellular_autom_mpi_exe $N 1000)
            if [ $? -eq 0 ]; then
                echo "MPI, $N, $i, $mpi_output" >> results_mpi.csv
            else
                echo "Fallo en la ejecución MPI para N = $N"
            fi
        done
        echo "" >> results_mpi.csv  
    done
    echo "Todas las iteraciones han finalizado con éxito."
else
    echo "MPI no está disponible, omitiendo la ejecución MPI."
fi

