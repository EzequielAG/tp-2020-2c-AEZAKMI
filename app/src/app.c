#include "app.h"
int main(void){

    app_init(&app_config, &logger);

    /*
    log_info(logger, "Soy el app! %s", mi_funcion_compartida());
    char* mensaje[3] = {"Hola", "Como", "Va"};
    send_messages_and_return_socket("127.0.0.1", "6011", mensaje, 3);
    */

    printf("Imprimiendo el path %s", app_config->ruta_log);

    app_finally(app_config, logger);
    return 0;
}

void app_init(t_app_config** app_config, t_log** logger){
    *app_config = app_config_loader("./cfg/app.config");
    *logger = log_create((*app_config)->ruta_log, "APP", true, LOG_LEVEL_INFO);
}

void app_finally(t_app_config* app_config, t_log* logger) {
    app_destroy(app_config);
    log_destroy(logger);
}

t_app_config* app_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_app_config* app_config = malloc(sizeof(t_app_config));

    app_config_parser(config, app_config);
    config_destroy(config);

    return app_config;
}


void app_config_parser(t_config* config, t_app_config* app_config) {
    app_config->ip_comanda = strdup(config_get_string_value(config, "IP_COMANDA"));
    app_config->puerto_comanda = strdup(config_get_string_value(config, "PUERTO_COMANDA"));
    app_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    app_config->grado_multiprocesamiento = config_get_int_value(config, "GRADO_DE_MULTIPROCESAMIENTO");
    app_config->algoritmo_planificacion = strdup(config_get_string_value(config, "ALGORITMO_DE_PLANIFICACION"));
    app_config->alpha = config_get_double_value(configuracion_get(), "ALPHA");
    app_config->estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
    app_config->repartidores = config_get_int_value(config, "REPARTIDORES");
    app_config->frecuencia_descanso = config_get_int_value(config, "FRECUENCIA_DE_DESCANSO");
    app_config->tiempo_descanso = config_get_int_value(config, "TIEMPO_DE_DESCANSO");
    app_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
    app_config->platos_default = config_get_int_value(config, "PLATOS_DEFAULT");
    app_config->posicion_rest_default_x = config_get_int_value(config, "POSICION_REST_DEFAULT_X");
    app_config->posicion_rest_default_y = config_get_int_value(config, "POSICION_REST_DEFAULT_Y");
}

void app_destroy(t_app_config* app_config) {
    free(app_config->ip_comanda);
    free(app_config->puerto_comanda);
    free(app_config->puerto_escucha);
    free(app_config->grado_multiprocesamiento);
    free(app_config->algoritmo_planificacion);
    //free(app_config->alpha);
    free(app_config->estimacion_inicial);
    free(app_config->repartidores);
    free(app_config->frecuencia_descanso);
    free(app_config->tiempo_descanso);
    free(app_config->ruta_log);
    free(app_config->platos_default);
    free(app_config->posicion_rest_default_x);
    free(app_config->posicion_rest_default_y);
    free(app_config);
}