#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
/*cabecera de tiempo de sitema*/
#include <ctime>
#include <iostream>
/* cabecera de las llamadas a MPI */
#include <mpi.h>

using namespace std;
/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por
 * el proceso 0, que los imprime
 */

int main(int argc, char** argv) { /* argc puntero al numero de argumentos, argv puntero a numero de vectores*/
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    int larnombre; /*Variable para guardar el nombre del procesador*/
    char mensaje[100]; /* mensaje  */
    MPI_Status estado; /* devuelve estado al recibir*/
    /*Se comienza el proceso de conseguir la fecha y se le asigna a la variable fecha*/
    time_t curr_time;
    tm * curr_tm;
    char fecha[100];
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    strftime(fecha, 50, "%m/%d/%Y %T", curr_tm);
    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango); // Obtenemos el valor de nuestro identificador


    /* Averiguamos el número de procesos que estan
     * ejecutando nuestro porgrama
     */
    char procesador_nombre[MPI_MAX_PROCESSOR_NAME]; //Se declara una variable char de tamaño maximo del nombre del procesador
    MPI_Comm_size(MPI_COMM_WORLD, &p); //Obtenemos el numero total de hebras
    MPI_Get_processor_name(procesador_nombre, &larnombre); // Obtenemos el nombre del procesador
    if (mi_rango != 0) {
        /* Crea mensaje */
        sprintf(mensaje,procesador_nombre);
        dest = 0; //
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,dest, tag, MPI_COMM_WORLD);
    }
    else { /* mi_rango == 0 */
      for (fuente = 1; fuente < p; fuente++) {
        MPI_Recv(mensaje, 100, MPI_CHAR, fuente,tag, MPI_COMM_WORLD, &estado);
        printf("%s %s\n", mensaje, fecha);
      }
    }
    MPI_Finalize(); //Se finaliza la ejecucion MPI (Siempre debe realizarse)
    return EXIT_SUCCESS;
}
