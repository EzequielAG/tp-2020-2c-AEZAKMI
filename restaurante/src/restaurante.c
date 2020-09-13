#include "restaurante.h"

int main(void){
//ESTE VA A ACTUAR DE SERVER
    restaurante_init(&restaurante_config, &logger);
    log_info(logger, "Soy el MODULO RESTAURANTE! %s", mi_funcion_compartida());
    printf("Imprimiendo el path %s", restaurante_config->ruta_log);

    restaurante_finally(restaurante_config, logger);
    return 0;
}

void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger){
    *restaurante_config = restaurante_config_loader("./cfg/restaurante.config");
    *logger = init_logger((*restaurante_config)->ruta_log, "restaurante", LOG_LEVEL_INFO);
}

void restaurante_finally(t_restaurante_config* restaurante_config, t_log* logger) {
    restaurante_destroy(restaurante_config);
    log_destroy(logger);
}

t_restaurante_config* restaurante_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_restaurante_config* restaurante_config = malloc(sizeof(t_restaurante_config));

    restaurante_config_parser(config, restaurante_config);
    config_destroy(config);

    return restaurante_config;
}

void restaurante_config_parser(t_config* config, t_restaurante_config* restaurante_config) {
    restaurante_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    restaurante_config->ip_sindicato = strdup(config_get_string_value(config, "IP_SINDICATO"));
    restaurante_config->puerto_sindicato = strdup(config_get_string_value(config, "PUERTO_SINDICATO"));
    restaurante_config->ip_app = strdup(config_get_string_value(config, "IP_APP"));
    restaurante_config->puerto_app = strdup(config_get_string_value(config, "PUERTO_APP"));
    restaurante_config->quantum = config_get_int_value(config, "QUANTUM");
    restaurante_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
    restaurante_config->algoritmo_planificador = strdup(config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
    restaurante_config->nombre_restaurante = strdup(config_get_string_value(config, "NOMBRE_RESTAURANTE"));
}

void restaurante_destroy(t_restaurante_config* restaurante_config) {
    free(restaurante_config->puerto_escucha);
    free(restaurante_config->ip_sindicato);
    free(restaurante_config->puerto_sindicato);
    free(restaurante_config->ip_app);
    free(restaurante_config->puerto_app);
    free(restaurante_config->ruta_log);
    free(restaurante_config->algoritmo_planificador);
    free(restaurante_config->nombre_restaurante);
    free(restaurante_config);
}

int obtener_id_pedido(){
    //TODO: Obtener un id de pedido unico para la instancia del restaurante
    int id_pedido = 435;
    return id_pedido;
}

void handle_crear_pedido(int socket){

    int id_pedido = obtener_id_pedido();
    char* respuesta[1];

    respuesta[0] = string_itoa(id_pedido);

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}