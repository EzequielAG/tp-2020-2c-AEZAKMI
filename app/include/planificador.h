#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "repartidor.h"
#include <pthread.h>
#include <unistd.h>

void iniciar_planificador();
void iniciar_repartidores();
void pcb_prueba();
void iniciar_planificador_corto_plazo();
void iniciar_planificador_largo_plazo();
void planificar_largo_plazo();
void iniciar_clock();
void clock_cpu();
void planificar_corto_plazo_FIFO();
#endif
