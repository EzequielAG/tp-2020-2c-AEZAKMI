#include "sindicato.h"
int main(void){
    sindicato_init(&sindicato_config, &logger);

    printf("Imprimiendo el path %s", sindicato_config->ruta_log);

    //TODO: Hacer que reciba ip y puerto de config
    iniciar_servidor("127.0.0.1", "5003", handle_client);

    sindicato_finally(sindicato_config, logger);
    return 0;
}

void sindicato_init(t_sindicato_config** sindicato_config, t_log** logger){
    *sindicato_config = sindicato_config_loader("./cfg/sindicato.config");
    *logger = init_logger((*sindicato_config)->ruta_log, "SINDICATO", LOG_LEVEL_INFO);
}

void sindicato_finally(t_sindicato_config* sindicato_config, t_log* logger) {
    sindicato_destroy(sindicato_config);
    log_destroy(logger);
}

t_sindicato_config* sindicato_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_sindicato_config* sindicato_config = malloc(sizeof(t_sindicato_config));

    sindicato_config_parser(config, sindicato_config);
    config_destroy(config);

    return sindicato_config;
}

void sindicato_config_parser(t_config* config, t_sindicato_config* sindicato_config) {
    sindicato_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    sindicato_config->punto_montaje = strdup(config_get_string_value(config, "PUNTO_MONTAJE"));
    sindicato_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
}

void sindicato_destroy(t_sindicato_config* sindicato_config) {
    free(sindicato_config->puerto_escucha);
    free(sindicato_config->punto_montaje);
    free(sindicato_config->ruta_log);
    free(sindicato_config);
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