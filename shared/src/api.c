#include "api.h"

void enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido){
    char* tipo_mensaje = string_itoa(guardar_pedido);

    char* guardad_pedido_mensajes[3] = {tipo_mensaje, restaurante, id_pedido};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, guardad_pedido_mensajes, 3);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
        printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");

    liberar_conexion(socket);
}