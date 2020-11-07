#ifndef PLANIFICACION_H
#define PLANIFICACION_H
#include "cocinero.h"


void inicializar_colas();
void inicializar_colas_ready();
void inicializar_colas_io();
void inicializar_colas_exec();
void inicializar_colas_block();
void inicializar_colas_exit();

typedef enum estado_proceso{
    READY = 1,
    READY_MAS = 2,
    EXEC = 3,
    BLOCKED = 4,
    IO = 5,
    EXIT = 6
} estado_proceso_t;

typedef struct{
    int pid;
    estado_proceso_t estado;
}
t_pcb;

typedef struct {
    t_pcb* pcb;
    char* nombre;
    List pasos;
}
t_plato;

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
int paso_block(t_plato* plato);
int paso_io(t_plato* plato);

List cola_ready_mas;
List cola_ready;
List cola_exec;
List cola_io;
List cola_block;
List cola_exit;
List* cola_cocineros;


#endif