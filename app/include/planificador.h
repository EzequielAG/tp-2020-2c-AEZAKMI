#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "repartidor.h"

typedef struct {
    int id_pedido;
    char* restaurante;
} t_pcb;

List suscriptores_cpu;
List pcb_new;
List pcb_ready;

void iniciar_planificador();
void iniciar_repartidores();
t_pcb* crear_pcb(char* restaurante, int id_pedido);
void pcb_prueba();
void iniciar_planificador_corto_plazo();
void iniciar_planificador_largo_plazo();
#endif
