#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"

typedef struct {
    int posx;
    int posy;
} t_posicion;

typedef struct {
    t_posicion posicion;
    int frecuencia_de_descanso;
    int tiempo_de_descanso;
} t_repartidor;

List suscriptores_cpu;

void iniciarPlanificador();
void iniciarRepartidores();

#endif
