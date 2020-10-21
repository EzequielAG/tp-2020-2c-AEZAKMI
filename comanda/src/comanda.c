#include "comanda.h"
int main(int argc, char *argv[]){

    comanda_init(&comanda_config, &logger);

    printf("Imprimiendo el path %s", comanda_config->ruta_log);
    iniciar_servidor("127.0.0.1", "5001", handle_client);

    //ASIGNAR SEGUN LA CONFIG

    puntero_memoria_principal = malloc(comanda_config->tamanio_memoria);

    comanda_finally(comanda_config, logger);
    return 0;
}

void comanda_init(t_comanda_config** comanda_config, t_log** logger){
    *comanda_config = comanda_config_loader("./cfg/comanda.config");
    *logger = init_logger((*comanda_config)->ruta_log, "comanda", LOG_LEVEL_INFO);
  
}

void comanda_finally(t_comanda_config* comanda_config, t_log* logger) {
    comanda_destroy(comanda_config);
    log_destroy(logger);
}

t_comanda_config* comanda_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_comanda_config* comanda_config = malloc(sizeof(t_comanda_config));

    comanda_config_parser(config, comanda_config);
    config_destroy(config);

    return comanda_config;
}

void comanda_config_parser(t_config* config, t_comanda_config* comanda_config) {
    comanda_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    comanda_config->esquema_memoria = strdup(config_get_string_value(config, "ESQUEMA_MEMORIA"));
    comanda_config->tamanio_memoria = config_get_int_value(config, "TAMANIO_MEMORIA");
    comanda_config->tamanio_swap = config_get_int_value(config, "TAMANIO_SWAP");
    comanda_config->frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
    comanda_config->algoritmo_reemplazo = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
    comanda_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
}

void comanda_destroy(t_comanda_config* comanda_config) {
    free(comanda_config->puerto_escucha);
    free(comanda_config->esquema_memoria);
    free(comanda_config->algoritmo_reemplazo);
    free(comanda_config->ruta_log);
    free(comanda_config);
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
            } else if (tipo_mensaje == guardar_plato){
            // TODO: FALTA LOGICA DE GUARDAR_PLATO
            } else if (tipo_mensaje == confirmar_pedido){
            // TODO: FALTA LOGICA DE CONFIRMAR_PEDIDO
            } else if (tipo_mensaje == plato_listo){
            // TODO: FALTA LOGICA DE PLATO_LISTO
            } else if (tipo_mensaje == obtener_pedido){
            // TODO: FALTA LOGICA DE OBTENER_PEDIDO
            } else if (tipo_mensaje == finalizar_pedido){
            // TODO: FALTA LOGICA DE FINALIZAR_PEDIDO
            }
 
        }
    }    
    





    return;
}


void handle_guardar_pedidos(int socket, char* restaurante, char* id_pedido){
    
    char* respuesta[1];

    if (guardar_pedido_en_memoria(restaurante, id_pedido)){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int guardar_pedido_en_memoria(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en memoria, 1 OK 0 FAIL
    
    return 1;
}