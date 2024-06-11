#!/bin/bash

# Compile the program
gcc -o cellular_autom_serial_exe cellular_autom_serial.c
echo "Compilación del programa serial completada."

# File to store the results
echo "Tipo, Tamaño, Iteraciones, Movimientos totales, Tiempo total, Velocidad promedio" > results_serial.csv

# Simulation parameters
sizes=(100000 200000 300000 400000 500000)

# Execute simulations
for i in {1..10}; do
    echo "Iniciando iteración $i de 10..."
    for N in "${sizes[@]}"; do
        serial_output=$(./cellular_autom_serial_exe $N 1000)
        echo "Serial, $N, $i, $serial_output" >> results_serial.csv
    done
    echo "" >> results_serial.csv  
done
echo "Todas las iteraciones han finalizado con éxito."
