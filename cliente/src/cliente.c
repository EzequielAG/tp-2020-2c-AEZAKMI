#include "cliente.h"
int main(int argc, char *argv[]){

    cliente_init(&cliente_config, &logger);
    log_info(logger, "Soy el CLIENTE! %s", mi_funcion_compartida());

    for (int i = 0; i < argc; i++){
        printf("%s\n", argv[i]);
    }

    if (argc < 2){
        printf("%s\n", "Debe indicar a qué modulo quiere conectarse");
        cliente_finally(cliente_config, logger);
        return -1;
    }

    t_modulo* modulo = get_modulo_by_name(argv[1]);

    if (modulo == NULL){
        printf("%s\n", "El modulo indicado no existe");
        cliente_finally(cliente_config, logger);
        return -1;
    }

    int resultado = handshake(modulo);

    if (resultado != 0){
        printf("%s\n", "No se pudo realizar la conexion con el cliente");
        cliente_finally(cliente_config, logger);
        return -1;
    }

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

t_modulo* get_modulo_by_name(char* nombre_del_modulo){
    t_modulo* modulo = NULL;
    if (!strcmp(nombre_del_modulo, "app")){
        modulo = crear_modulo(cliente_config->ip_app, cliente_config->puerto_app, "app");
    }
    return modulo;
}

t_modulo * crear_modulo(char* ip, char* puerto, char* nombre){
    t_modulo* modulo = malloc(sizeof(t_modulo));
    modulo->ip = ip;
    modulo->puerto = puerto;
    modulo->nombre = nombre;
    return modulo;
}

int handshake(t_modulo* modulo){
    int socket = send_message_and_return_socket(modulo->ip, modulo->puerto, "HANDSHAKE");

    if (socket == -1){
        return -1;
    }

    char * mensaje = receive_simple_message(socket);

    if (mensaje == NULL){
        return -1;
    }

    printf("El handshake con el modulo %s fue correcto\n", modulo->nombre);

    return 0;
}