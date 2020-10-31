#ifndef SINDICATO_H
#define SINDICATO_H

#include "sindicato_fs.h"

// VARIABLES Y ESTRUCTURAS

// FUNCIONES
int guardar_pedido_en_afip(char* restaurante, char* id_pedido);
void handle_guardar_pedido(int socket, char* restaurante, char* id_pedido);
void handle_client(t_result* result);
void handle_consultar_platos(int socket, char* restaurante);
void handle_guardar_plato(int socket, char* restaurante, char* id_pedido, char* comida, char* cantidad);
void handle_confirmar_pedido(int socket, char* id_pedido,  char* restaurante);
void handle_obtener_pedido(int socket, char* restaurante,  char* id_pedido);
void handle_obtener_restaurante(int socket, char* restaurante);
void handle_plato_listo(int socket, char* restaurante,  char* id_pedido, char* comida);
void handle_obtener_receta(int socket, char* comida);
void handle_terminar_pedido(int socket, char* id_pedido,  char* restaurante);
void handle_error(int socket);

#endif