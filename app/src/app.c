#include "app.h"
int main(void){

    app_init(&app_config, &logger);

    iniciar_servidor("127.0.0.1", "5004", handle_client);

    app_finally(app_config, logger);
    return 0;
}

void handle_client(t_result* result){

    
    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        if (tipo_mensaje == handshake_cliente){
            handle_handshake_cliente(result->socket, result->mensajes->mensajes[1]);
        } else if (tipo_mensaje == handshake_restaurante) {
            handle_handshake_restaurante(result->socket, result->mensajes->mensajes[1]);
        } else if (tipo_mensaje == consultar_restaurantes){
            handle_consultar_restaurantes(result->socket);
        } else if (tipo_mensaje == seleccionar_restaurante){
            handle_seleccionar_restaurante(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
        } else if (tipo_mensaje == crear_pedido){
            handle_crear_pedido(result->socket);
        }
    }
    
    return;
}

void handle_handshake_restaurante(int socket, char* nombre_restaurante){
    t_restaurante* restaurante = buscar_restaurante_lista(nombre_restaurante);
            
    if (restaurante == NULL){
        restaurante = nuevo_restaurante(socket, nombre_restaurante);
        pushbacklist(&lista_restaurantes, restaurante);
    } else {
        restaurante->socket = socket;
    }
    
    send_message_socket(socket, "OK");
}

t_restaurante* nuevo_restaurante(int socket, char* nombre_restaurante){
    t_restaurante* restaurante = malloc(sizeof(t_restaurante));
    restaurante->socket = socket;
    restaurante->nombre_restaurante = string_new();
    string_append(&restaurante->nombre_restaurante, nombre_restaurante);

    return restaurante;
}

t_restaurante* buscar_restaurante_lista(char* nombre_restaurante){

    for (IteratorList iter = beginlist(lista_clientes); iter != NULL; iter = nextlist(iter)){
        t_restaurante* restaurante = (t_restaurante*) iter->data;

        if (strcmp(restaurante->nombre_restaurante, nombre_restaurante) == 0){
            return restaurante;
        }

    }

    return NULL;

}

void handle_handshake_cliente(int socket, char* id_cliente){
    t_cliente* cliente = buscar_cliente_lista(id_cliente);
            
    if (cliente == NULL){
        cliente = malloc(sizeof(t_cliente));
        cliente->socket = socket;
        cliente->id_cliente = string_new();
        string_append(&cliente->id_cliente, id_cliente);
        pushbacklist(&lista_clientes, cliente);
    } else {
        cliente->socket = socket;
    }
    
    send_message_socket(socket, "OK");
}

t_cliente* buscar_cliente_lista(char* id_cliente){

    for (IteratorList iter = beginlist(lista_clientes); iter != NULL; iter = nextlist(iter)){
        t_cliente* cliente = (t_cliente*) iter->data;

        if (strcmp(cliente->id_cliente, id_cliente) == 0){
            return cliente;
        }

    }

    return NULL;

}

void handle_crear_pedido(int socket){

    int id_pedido = obtener_id_pedido();
    
    t_modulo modulo_comanda;
    modulo_comanda.ip = app_config->ip_comanda;
    modulo_comanda.puerto = app_config->puerto_comanda;
    modulo_comanda.nombre = "Comanda";

    char* respuesta[1];
    respuesta[0] = string_itoa(id_pedido);

    //TODO: De donde saco el restaurante?
    enviar_mensaje_guardar_pedido(&modulo_comanda, "Restaurante1", respuesta[0]);

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int obtener_id_pedido(){
    //TODO: Obtener un id de pedido, enviando un mensaje al restaurante
    int id_pedido = 435;
    return id_pedido;
}

void handle_seleccionar_restaurante(int socket, char* restaurante, char* cliente){

    //TODO: Cambiar para leer los restaurantes reales
    char* respuesta[1];

    if (relacionar(restaurante, cliente)){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int relacionar(char* restaurante, char* cliente){
    //TODO hacer que se relacionen el cliente y el restaurante, devolver 0 ante error
    return 1;
}

void handle_consultar_restaurantes(int socket){

    char* restaurantes = obtener_restaurantes();
    //TODO: Cambiar para leer los restaurantes reales
    char* respuesta[1] = {restaurantes};

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

char* obtener_restaurantes(){

    char * restaurantes = string_new();

    string_append(&restaurantes, "[");
    int primero = 1;
    for (IteratorList iter = beginlist(lista_restaurantes); iter != NULL; iter = nextlist(iter)){
        if (primero == 1){
            primero = 0;
        } else {
            string_append(&restaurantes, ",");
        }
        t_restaurante* restaurante = (t_restaurante*) iter->data;
        string_append(&restaurantes, restaurante->nombre_restaurante);

    }

    string_append(&restaurantes, "]");

    return restaurantes;
}

void app_init(t_app_config** app_config, t_log** logger){

    //INICIALIZO LISTAS DE RESTAURANTES Y CLIENTES
    initlist(&lista_clientes);
    initlist(&lista_restaurantes);

    //INSERTO RESTAURANTE DEFAULT
    t_restaurante* restaurante = nuevo_restaurante(0, "Resto Default");
    pushbacklist(&lista_restaurantes, restaurante);

    *app_config = app_config_loader("./cfg/app.config");
    *logger = init_logger((*app_config)->ruta_log, "APP", LOG_LEVEL_INFO);
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
    app_config->alpha = config_get_double_value(config, "ALPHA");
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
    free(app_config->algoritmo_planificacion);
    free(app_config->ruta_log);
    free(app_config);
}