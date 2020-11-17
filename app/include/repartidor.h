#ifndef REPARTIDOR_H
#define REPARTIDOR_H

#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"
#include <semaphore.h>

typedef struct t_repartidor t_repartidor;

typedef struct {
    int id_pedido;
    char* restaurante;
    t_repartidor* repartidor_actual;
} t_pcb;

List suscriptores_cpu;
sem_t* sem_pcb_new;
List pcb_new;
sem_t* sem_pcb_ready;
List pcb_ready;
sem_t* sem_grado_multiprocesamiento;

typedef struct {
    int posx;
    int posy;
} t_posicion;

typedef struct {
    t_posicion posicion_restaurante;
    t_posicion posicion_cliente;
} t_pedido;

struct t_repartidor {
    int id;
    t_posicion posicion;
    int frecuencia_de_descanso;
    int tiempo_de_descanso;
    sem_t* nuevo_pedido;
    sem_t* ciclo_cpu;
    t_pedido* pedido;
    int cansancio;
    t_pcb* pcb_actual;
};

List repartidores_libres;
sem_t* sem_entrenador_libre;


void repartir_pedidos(t_repartidor* repartidor);
void repartir_pedido(t_repartidor* repartidor);
void ir_hacia_restaurante(t_repartidor* repartidor);
void esperar_pedido(t_repartidor* repartidor);
void ir_hacia_cliente(t_repartidor* repartidor);
void entregar_pedido(t_repartidor* repartidor);
bool misma_posicion(t_posicion posicion1, t_posicion posicion2);
void avanzar_hacia(t_repartidor* repartidor, t_posicion destino);
void mover_hacia_izquierda(t_repartidor* repartidor);
void mover_hacia_derecha(t_repartidor* repartidor);
void mover_hacia_abajo(t_repartidor* repartidor);
void mover_hacia_arriba(t_repartidor* repartidor);
void cansarse(t_repartidor* repartidor);
bool esta_cansado(t_repartidor* repartidor);
void descansar(t_repartidor* repartidor);
void buscar_datos_pedido(t_repartidor* repartidor);
void desuscribirse_clock(sem_t* ciclo_cpu);
void pasar_a_ready(t_repartidor* repartidor);

#endif
