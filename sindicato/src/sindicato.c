#include "sindicato.h"
int main(void){
    sindicato_init(&sindicato_config, &logger);
    get_or_create_fs();

    printf("Imprimiendo el path %s", sindicato_config->ruta_log);

    //TODO: Hacer que reciba ip y puerto de config
    iniciar_servidor("127.0.0.1", "5003", handle_client);

    sindicato_finally(sindicato_config, logger);
    return 0;
}

void handle_client(t_result* result){
    
    if (result->operacion == MENSAJE){
        if (!strcmp(result->mensaje, "HANDSHAKE")){
            send_message_socket(result->socket, "OK");
            liberar_conexion(result->socket);
        }
    } else {
        if (result->operacion == MENSAJES){
            int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
            if (tipo_mensaje == guardar_pedido){
                handle_guardar_pedidos(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
            } 
        }
    }    
    
    return;
}


void handle_guardar_pedidos(int socket, char* restaurante, char* id_pedido){
    
    char* respuesta[1];

    if (guardar_pedido_en_afip(restaurante, id_pedido)){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int guardar_pedido_en_afip(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en afip, 1 OK 0 FAIL
    
    return 1;
}