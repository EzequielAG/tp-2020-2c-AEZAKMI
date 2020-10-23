#ifndef SINDICATO_H
#define SINDICATO_H

#include "sindicato_fs.h"

// VARIABLES Y ESTRUCTURAS

// FUNCIONES
int guardar_pedido_en_afip(char* restaurante, char* id_pedido);
void handle_guardar_pedidos(int socket, char* restaurante, char* id_pedido);
void handle_client(t_result* result);

#endif