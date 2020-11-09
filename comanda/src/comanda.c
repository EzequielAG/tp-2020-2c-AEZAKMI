#include "comanda.h"

int main(int argc, char *argv[]){

    comanda_init(&comanda_config, &logger);
	
    initlist(&tablaRestaurantes);
    initlist(&tablaFrames);
    initlist(&tablaSwap);
    initlist(&pilaPaginasAlgoritmos);

	punteroBitMap = malloc((comanda_config->tamanio_memoria/256));
	punteroBitMapSwap = malloc(comanda_config->tamanio_swap/256);

	bitMap = bitarray_create_with_mode(punteroBitMap, (comanda_config->tamanio_memoria/256), MSB_FIRST);
    bitMapSwap = bitarray_create_with_mode(punteroBitMapSwap, comanda_config->tamanio_swap/256, MSB_FIRST);

    puntero_memoria_principal = malloc(comanda_config->tamanio_memoria);

    algoritmo = comanda_config->algoritmo_reemplazo;
    tamanioMemoria = comanda_config->tamanio_memoria;
    tamanioSwap = comanda_config->tamanio_swap;
    
    printf("Imprimiendo el path %s", comanda_config->ruta_log);

    signal(SIGUSR1,&imprimirBitMap);

    iniciarMemoria();

    iniciarMemoriaSwap();
    
    //iniciar_servidor("127.0.0.1", "5001", handle_client);

    //ASIGNAR SEGUN LA CONFIG

    //iniciarMemoria();

    char *string = "Lo de tito";
    char* numero = "5";

    guardar_pedido_en_memoria(string, numero);
    guardar_pedido_en_memoria(string, "7"); 
    guardar_pedido_en_memoria("Lo de nacho", "6");
    guardar_pedido_en_memoria("Lo de nacho", "8");
    guardar_pedido_en_memoria("Lo de nacho", "9");
    guardar_pedido_en_memoria("Lo de nacho", "10");
    guardar_pedido_en_memoria("Lo de nacho", "11");
    guardar_pedido_en_memoria("Lo de nacho", "12");
    guardar_pedido_en_memoria("Lo de nacho", "13");
    guardar_pedido_en_memoria("Lo de nacho", "14");
    

    guardar_plato_en_memoria(string, numero, "4", "Arroz");
    guardar_plato_en_memoria(string, "7", "4", "Arroz con pollo");
    guardar_plato_en_memoria("Lo de nacho", "6", "3", "Pollito bien fresco");
    guardar_plato_en_memoria("Lo de nacho", "13", "4", "Curcuma escurtida");
    guardar_plato_en_memoria("Lo de nacho", "8", "4", "Cebollita caramelizada");
    guardar_plato_en_memoria("Lo de nacho", "9", "4", "Pepinos");
    guardar_plato_en_memoria("Lo de nacho", "11", "4", "Puede serphite");
    guardar_plato_en_memoria("Lo de nacho", "10", "4", "Hamburguesa completa");
    guardar_plato_en_memoria("Lo de nacho", "10", "3", "Hamburguesa completa");
    guardar_plato_en_memoria("Lo de nacho", "11", "2", "Puede serphite");
    guardar_plato_en_memoria("Lo de nacho", "12", "4", "Ensalada de tomate");

    finalizar_pedido_en_memoria(string, numero);

    //guardar_plato_en_memoria("Lo de nacho", "14", "4", "Pure de papa");

    //guardar_plato_en_memoria("Lo de nacho", "6", "3", "Pollito bien fresco");
    //guardar_plato_en_memoria("Lo de tito", "5", "4", "Arroz");

    imprimirBitMap();

    imprimirMemoria();

    imprimirSwap();

    //imprimirTodo();

    close(archivoSwap);

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
    comanda_config->algoritmo_reemplazo = strdup(config_get_string_value(config, "ALGORITMO_REEMPLAZO"));
    comanda_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
}

void comanda_destroy(t_comanda_config* comanda_config) {
    free(comanda_config->puerto_escucha);
    free(comanda_config->esquema_memoria);
    free(comanda_config->algoritmo_reemplazo);
    free(comanda_config->ruta_log);
    free(comanda_config);
}

