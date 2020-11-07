#ifndef PLANIFICACION_H
#define PLANIFICACION_H
#include "cocinero.h"


void inicializar_colas();
void inicializar_colas_ready();
void inicializar_colas_io();

typedef struct {
int id;
char* nombre;
List pasos;
}t_plato;

typedef struct{
    char* nombre;
    int ciclos_cpu;
    int se_ejecuto;
    int es_io;
}
t_pasos_platos;

typedef struct{
    int ocupado;
    t_plato* plato;
} t_horno;

int paso_ready(t_plato* plato);

List cola_ready_mas;
List cola_ready;
List cola_io;
List* cola_cocineros;
#endif