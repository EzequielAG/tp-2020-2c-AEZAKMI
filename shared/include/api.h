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
    consultar_pedido = 11,
    obtener_pedido = 12,
    finalizar_pedido = 13,
    terminar_pedido = 14,
    obtener_receta = 15,
    handshake_cliente = 16
} tipo_mensaje_t;


typedef struct {
    char* ip;
    char* puerto;
    char* nombre;
} t_modulo;


void leer_consola(t_log* logger,t_modulo* modulo);
void enviar_mensajes_por_consola(t_modulo* modulo, char* mensaje_completo);


int obtener_numero_mensaje(char* mensaje_tipo);

void enviar_mensaje_consultar_restaurantes(t_modulo* modulo);
void enviar_mensaje_seleccionar_restaurante(t_modulo* modulo, char* restaurante);
void enviar_mensaje_obtener_restaurante(t_modulo* modulo, char* restaurante);
void enviar_mensaje_consultar_platos(t_modulo* modulo, char* restaurante);
void enviar_mensaje_crear_pedido(t_modulo* modulo);
void enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido);
void enviar_mensaje_anadir_plato(t_modulo* modulo, char* plato, char* id_pedido);
void enviar_mensaje_guardar_plato(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida, char* cantidad);
void enviar_mensaje_confirmar_pedido(t_modulo* modulo,char* id_pedido, char* restaurante);
void enviar_mensaje_plato_listo(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida);
void enviar_mensaje_consultar_pedido(t_modulo* modulo, char* id_pedido);
void enviar_mensaje_obtener_pedido(t_modulo* modulo, char* id_pedido,char* restaurante);
void enviar_mensaje_finalizar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante);
void enviar_mensaje_terminar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante);
void enviar_mensaje_obtener_receta(t_modulo* modulo, char* nombre_plato);


#endif