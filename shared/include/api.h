#ifndef API_H
#define API_H
#include "server.h"
#include <commons/string.h>

typedef enum tipo_mensaje{
    consultar_restaurantes = 1,
    seleccionar_restaurante = 2,
    obtener_restaurante = 3,
    consultar_platos = 4,
    crear_pedido = 5,
    guardar_pedido = 6,
    anadir_plato = 7,
    guardar_plato = 8,
    confirmar_pedido = 9,
    plato_listo = 10,
    finalizar_pedido = 11,
    terminar_pedido = 12,
    obtener_receta = 13
} tipo_mensaje_t;

typedef struct {
    char* ip;
    char* puerto;
    char* nombre;
} t_modulo;


void enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido);

#endif