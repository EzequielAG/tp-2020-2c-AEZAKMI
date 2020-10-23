#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"
#include "api.h"
#include "memoria_principal.h" //Por handle de memoria


void handle_client(t_result* result);
int guardar_pedido_en_memoria(char* restaurante, char* id_pedido);
void handle_guardar_pedidos(t_result*);
void handle_guardar_plato(char* nombreResto, char*, char* cantidadPlato, char* plato);
void handle_confirmar_pedido(t_result *);
void handle_plato_listo(t_result*);
void handle_obtener_pedido(t_result *);
void handle_finalizar_pedido(t_result *);
