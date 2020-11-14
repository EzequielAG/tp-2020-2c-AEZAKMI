#include "app_messages.h"

void handle_client(t_result* result){
    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        switch(tipo_mensaje){
            case handshake_cliente: 
                handle_handshake_cliente(result->socket, result->mensajes->mensajes[1]);
            break;
            case handshake_restaurante:
                handle_handshake_restaurante(result->socket, result->mensajes->mensajes[1]);
            break;
            case consultar_restaurantes:
                handle_consultar_restaurantes(result->socket);
            break;
            case seleccionar_restaurante:
                handle_seleccionar_restaurante(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
            break;
            case consultar_platos: 
                handle_consultar_platos(result->socket, result->identificador_cliente);
            break;
            case crear_pedido:
                handle_crear_pedido(result->socket);
            break;
            case anadir_plato:
                // TODO: FALTA LOGICA DE ANADIR_PLATO
            break;
            case plato_listo:
                // TODO: FALTA LOGICA DE PLATO_LISTO
            break;
            case consultar_pedido:
                // TODO: FALTA LOGICA DE CONSULTAR_PEDIDO
            break;

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

    for (IteratorList iter = beginlist(lista_restaurantes); iter != NULL; iter = nextlist(iter)){
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
    modulo_comanda.ip = ip_comanda;
    modulo_comanda.puerto = puerto_comanda;
    modulo_comanda.identificacion = "APP";

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

void handle_seleccionar_restaurante(int socket, char* cliente, char* restaurante){

    //TODO: Cambiar para leer los restaurantes reales
    char* respuesta[1];

    if (relacionar(restaurante, cliente) != -1){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
}

int relacionar(char* nombre_restaurante, char* id_cliente){

    t_restaurante* restaurante = buscar_restaurante_lista(nombre_restaurante);

    if (restaurante == NULL){
        return -1;
    }

    t_cliente* cliente = buscar_cliente_lista(id_cliente);
    if (cliente == NULL){
        return -1;
    }

    cliente->restaurante = restaurante;

    return 0;
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

handle_consultar_platos(int socket, char* idCliente){

    //char* respuesta[1];

    t_cliente* cliente = buscar_cliente_lista(idCliente);
    t_restaurante* restaurante = cliente->restaurante;

    //enviar_mensaje_consultar_platos(w, NULL);

    //send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);

}