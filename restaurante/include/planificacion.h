#ifndef PLANIFICACION_H
#define PLANIFICACION_H
#include "cocinero.h"


void inicializar_colas();

void inicializar_colas_io();

typedef enum estado_proceso{
    NEW = 0,
    READY = 1,
    EXEC = 2,
    BLOCKED = 3,
    EXIT = 4
} estado_proceso_t;

typedef struct{
    int pid;
    int id_pedido;
    estado_proceso_t estado;
} t_pcb;

typedef struct {
    t_pcb* pcb;
    char* nombre;
    List* pasos;
    int cantidad_total;
    int cantidad_listo;
} t_plato;

typedef struct {
    void* afinidad;
    t_plato* plato_en_ejecucion;

} t_cocinero;

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

typedef struct{
    t_cocinero* cocinero;
    int ocupado;
    t_plato* plato;

}t_exec;

typedef struct{
    char* afinidad;
    List* platos_espera;
    t_exec* puntero_exec;

}t_ready;


typedef struct{
    t_horno** horno;
    List* platos_espera;

}t_io;


int paso_new_a_ready();
int paso_ready(t_plato* plato);
int paso_block(t_plato* plato);
int paso_io(t_plato* plato);
int paso_new(t_plato* plato);

int inicializar_colas_ready_exec();
List* cola_cocineros;
t_ready** cola_ready;


#endif