#ifndef COCINERO_H
#define COCINERO_H
#include "shared_utils.h"
#include "api.h"

//VARIABLES GLOBALES
List* afinidades;
char* pos_x;
char* pos_y;
receta_precio** recetas;
char* cantidad_hornos;
char* cantidad_pedidos;
char* cantidad_cocineros;
int socket_sindicato;
t_modulo modulo_sindicato;

void cocineroMain();

#endif