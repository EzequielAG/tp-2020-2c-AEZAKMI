#include "cliente.h"
int main(int argc, char *argv[]){
    cliente_init(&cliente_config, &logger);

    /*
    log_info(logger, "Soy el CLIENTE! %s", mi_funcion_compartida());
    char* mensaje[3] = {"Hola", "Como", "Va"};
    send_messages_and_return_socket("127.0.0.1", "6011", mensaje, 3);
    */

    printf("Imprimiendo el path %s", cliente_config->ruta_log);

    cliente_finally(cliente_config, logger);
    return 0;
}

void cliente_init(t_cliente_config** cliente_config, t_log** logger){
    *cliente_config = cliente_config_loader("./cfg/cliente.config");
    *logger = init_logger((*cliente_config)->ruta_log, "CLIENTE", LOG_LEVEL_INFO);
}

void cliente_finally(t_cliente_config* cliente_config, t_log* logger) {
    cliente_destroy(cliente_config);
    log_destroy(logger);
}

t_cliente_config* cliente_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_cliente_config* cliente_config = malloc(sizeof(t_cliente_config));

    cliente_config_parser(config, cliente_config);
    config_destroy(config);

    return cliente_config;
}

void cliente_config_parser(t_config* config, t_cliente_config* cliente_config) {
    cliente_config->ip_comanda = strdup(config_get_string_value(config, "IP_COMANDA"));
    cliente_config->puerto_comanda = strdup(config_get_string_value(config, "PUERTO_COMANDA"));
    cliente_config->ip_restaurante = strdup(config_get_string_value(config, "IP_RESTAURANTE"));
    cliente_config->puerto_restaurante = strdup(config_get_string_value(config, "PUERTO_RESTAURANTE"));
    cliente_config->ip_sindicato = strdup(config_get_string_value(config, "IP_SINDICATO"));
    cliente_config->puerto_sindicato = strdup(config_get_string_value(config, "PUERTO_SINDICATO"));
    cliente_config->ip_app = strdup(config_get_string_value(config, "IP_APP"));
    cliente_config->puerto_app = strdup(config_get_string_value(config, "PUERTO_APP"));
    cliente_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
    cliente_config->posicion_x = config_get_int_value(config, "POCISION_X");
    cliente_config->posicion_y = config_get_int_value(config, "POCISION_Y");
}

void cliente_destroy(t_cliente_config* cliente_config) {
    free(cliente_config->ip_comanda);
    free(cliente_config->puerto_comanda);
    free(cliente_config->ip_restaurante);
    free(cliente_config->puerto_restaurante);
    free(cliente_config->ip_sindicato);
    free(cliente_config->puerto_sindicato);
    free(cliente_config->ip_app);
    free(cliente_config->puerto_app);
    free(cliente_config->ruta_log);
    free(cliente_config);
}