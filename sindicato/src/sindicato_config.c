#include "sindicato_config.h"

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
