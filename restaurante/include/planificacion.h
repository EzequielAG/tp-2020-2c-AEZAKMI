#ifndef PLANIFICACION_H
#define PLANIFICACION_H
#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <semaphore.h>
#include "server.h"
#include "tests.h"
#include "shared_utils.h"
#include "api.h"


//VARIABLES GLOBALES
List afinidades;
List lista_pedidos;
char* pos_x;
char* pos_y;
int cantidad_hornos;
int cantidad_pedidos;
int cantidad_platos;
int pid;
int cantidad_cocineros;
int socket_sindicato;
int socket_app;
t_modulo modulo_sindicato;
t_modulo modulo_app;
receta_precio** recetas;
sem_t* sem_exec;


void inicializar_colas();

void inicializar_colas_io();

typedef enum estado_proceso{
    NEW = 0,
    READY = 1,
    EXEC = 2,
    BLOCKED = 3,
    BLOCKED_SUSPENDED = 4,
    EXIT = 5
} estado_proceso_t;


typedef struct {
    
    char* nombre;
    List pasos;
    int cantidad_total;
    int cantidad_listo;
 
} t_plato;

typedef struct{
    int ocupado;
    t_plato* plato;

}t_exec;


typedef struct {
    int id_pedido;
    List platos;
} t_pedido;

typedef struct{
    int ocupado;
    t_plato* plato;

} t_horno;

typedef struct{
    char* afinidad;
    List platos_espera;
    t_exec* puntero_exec;
    
}t_ready;


typedef struct{
    List hornos;
    List platos_espera;
}t_io;

typedef struct{
    int id_pedido;
    int pid;
    estado_proceso_t estado;
    t_plato* plato;
    t_ready* cola_ready_perteneciente;
} t_pcb;




List colas_ready;
List colas_exit;
List colas_pcb;
t_io* cola_io;

void planificacion_fifo();
int ultimo_paso(t_pcb* pcb);
int inicializar_colas_ready_exec();
void ocupar_horno_libre();
int horno_libre();
int paso_ready(t_pcb* pcb);
int paso_exit(t_pcb* pcb);
t_horno* paso_block(t_pcb* pcb);
int paso_exec(t_pcb* pcb);
int ejecutar_ciclo(t_pcb* pcb,t_paso* paso);
t_ready* asignar_cola_ready(t_plato* plato);
int es_paso_io(t_paso* paso);
t_exec* crear_exec();
int pasos_ejecutados(t_pcb* pcb);
int termino_pedido(int id_pedido);
char* obtener_estado(int estado);
List* hilos_ready();
void sacar_exec(t_pcb* pcb);
void sacar_ready(t_pcb* pcb);
void sacar_horno(t_pcb* pcb);

t_paso* crear_paso(char* nombre_paso, int ciclo_cpu);
t_plato* crear_plato(char* nombre, List* pasos, int pedido_id, int cantidad_total, int cantidad_listo, int pid);
t_pedido* creacion_pedido(int id, List* platos);
t_pcb* crear_pcb(int id_pedido,int pid, int estado,t_plato* plato);

int ejecutar_ciclos_fifo(t_pcb* pcb);
#endif
