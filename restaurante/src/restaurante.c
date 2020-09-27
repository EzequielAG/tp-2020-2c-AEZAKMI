#include "restaurante.h"

int main(void){
//ESTE VA A ACTUAR DE SERVER
    restaurante_init(&restaurante_config, &logger);
    log_info(logger, "Soy el MODULO RESTAURANTE! %s", mi_funcion_compartida());
    printf("Imprimiendo el path %s", restaurante_config->ruta_log);


    t_modulo modulo_app = {restaurante_config->ip_app, restaurante_config->puerto_app, "app"};

    int respuesta = handshake(&modulo_app);

    if (respuesta == -1){
        printf("No se pudo realizar la conexion inicial con el modulo app\n");
        return -1;
    }

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

    t_modulo modulo_sindicato;
    modulo_sindicato.ip = restaurante_config->ip_sindicato;
    modulo_sindicato.puerto = restaurante_config->puerto_sindicato;
    modulo_sindicato.nombre = "Comanda";

    //TODO: De donde saco el restaurante?
    enviar_mensaje_guardar_pedido(&modulo_sindicato, "Restaurante1", respuesta[0]);

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int handshake(t_modulo* modulo){

    char* mensajes[2] = {string_itoa(handshake_restaurante), restaurante_config->nombre_restaurante};

    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, mensajes, 2);

    if (socket == -1){
        return -1;
    }

    char * mensaje = receive_simple_message(socket);

    if (mensaje == NULL){
        return -1;
    }

    printf("El handshake con el modulo %s fue correcto\n", modulo->nombre);

    escuchar_mensajes_socket_desacoplado(socket);

    return 0;
}

void escuchar_mensajes_socket_desacoplado(int socket){
    
    pthread_t thread;
    t_parameter* parametro = malloc(sizeof(t_parameter));

	parametro->socket = socket;
	parametro->f = handle_client;

	pthread_create(&thread,NULL,(void*)escuchar_mensajes_socket, parametro);
	pthread_detach(thread);

}

void escuchar_mensajes_socket(t_parameter* parametro){
    escuchar_socket(&parametro->socket, parametro->f);
}

void handle_client(t_result* result){

    for(int i = 0; i < *result->mensajes->size; i++){
        printf("%s", result->mensajes->mensajes[i]);
    }
    printf("\n");

    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        if (tipo_mensaje == consultar_platos){
            // TODO : FALTA LOGICA CONSULTAR_PLATOS
        } else if (tipo_mensaje == crear_pedido) {
            // TODO : FALTA LOGICA CREAR_PEDIDO
        } else if (tipo_mensaje == anadir_plato) {
            // TODO : FALTA LOGICA ANADIR_PLATO
        } else if (tipo_mensaje == confirmar_pedido) {
            // TODO : FALTA LOGICA CONFIRMAR_PEDIDO
        } else if (tipo_mensaje == consultar_pedido) {
            // TODO : FALTA LOGICA CONSULTAR_PEDIDO
        }

    }

}