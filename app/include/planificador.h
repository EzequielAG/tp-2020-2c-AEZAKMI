#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "list.h"
#include "config_app.h"
#include <commons/string.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "repartidor.h"

List suscriptores_cpu;

void iniciarPlanificador();
void iniciarRepartidores();

#endif
