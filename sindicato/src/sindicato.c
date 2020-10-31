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
            switch(tipo_mensaje)
            {
                case consultar_platos:
                    handle_consultar_platos(result->socket, result->mensajes->mensajes[1]);
                    break;
                case guardar_pedido:
                    handle_guardar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case guardar_plato:
                    handle_guardar_plato(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3], result->mensajes->mensajes[4]);
                    break;
                case confirmar_pedido:
                    handle_confirmar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case obtener_pedido:
                    handle_obtener_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case obtener_restaurante:
                    handle_obtener_restaurante(result->socket, result->mensajes->mensajes[1]);
                    break;
                case plato_listo:
                    handle_plato_listo(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3]);
                    break;
                case obtener_receta:
                    handle_obtener_receta(result->socket, result->mensajes->mensajes[1]);
                    break;
                case terminar_pedido:
                    handle_terminar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                default:
                    handle_error(result->socket);
                    break;
            }
    
        }
    }    

    liberar_conexion(result->socket);
    return;
}

int guardar_pedido_en_afip(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en afip, 1 OK 0 FAIL
    
    return 1;
}

void handle_consultar_platos(int socket, char* restaurante){

}

void handle_guardar_pedido(int socket, char* restaurante, char* id_pedido){
    
    char* respuesta[1];

    if (guardar_pedido_en_afip(restaurante, id_pedido)){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

void handle_guardar_plato(int socket, char* restaurante, char* id_pedido, char* comida, char* cantidad){

}

void handle_confirmar_pedido(int socket, char* id_pedido,  char* restaurante){

}

void handle_obtener_pedido(int socket, char* restaurante,  char* id_pedido){

}

void handle_obtener_restaurante(int socket, char* restaurante){

}

void handle_plato_listo(int socket, char* restaurante,  char* id_pedido, char* comida){

}

void handle_obtener_receta(int socket, char* comida){

}

void handle_terminar_pedido(int socket, char* id_pedido,  char* restaurante){

}

void handle_error(int socket){
    char* respuesta[1];
    respuesta[0] = "ERROR";
    send_messages_socket(socket, respuesta, 1);
}