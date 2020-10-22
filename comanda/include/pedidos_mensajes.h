#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"
#include "api.h"

typedef struct pedidoRecibido{

    int socket;
    char* nombreResto;
    char* idPedido;
    char* plato;
    int cantidadPlato;

} l_pedidoRecibido;



void handle_client(t_result* result);
int guardar_pedido_en_memoria(char* restaurante, char* id_pedido);
void handle_guardar_pedidos(l_pedidoRecibido*);
void handle_guardar_plato(l_pedidoRecibido *pedidos);
void handle_confirmar_pedido(l_pedidoRecibido *pedidos);
void handle_plato_listo(l_pedidoRecibido *pedidos);
void handle_obtener_pedido(l_pedidoRecibido *pedidos);
void handle_finalizar_pedido(l_pedidoRecibido *pedidos);
