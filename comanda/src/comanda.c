#include "comanda.h"

int main(int argc, char *argv[]){

    comanda_init(&comanda_config, &logger);
	
    initlist(&tablaRestaurantes);

    puntero_memoria_principal = malloc(comanda_config->tamanio_memoria);
    
    printf("Imprimiendo el path %s", comanda_config->ruta_log);
    iniciar_servidor("127.0.0.1", "5001", handle_client);

    //ASIGNAR SEGUN LA CONFIG



    // guardar_pedido_en_memoria("Lo de tito", "5");
    // guardar_pedido_en_memoria("Lo de tito", "7");
    // guardar_pedido_en_memoria("Lo de nacho", "6");

    // guardar_plato_en_memoria("Lo de tito", "5", "4", "Arroz");
    // guardar_plato_en_memoria("Lo de tito", "7", "4", "Arroz con pollo");
    // guardar_plato_en_memoria("Lo de nacho", "6", "3", "Pollito bien fresco");


    imprimirMemoria();

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

