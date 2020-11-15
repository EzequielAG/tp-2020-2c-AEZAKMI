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
                handle_crear_pedido(result->socket, result->identificador_cliente);
            break;
            case anadir_plato:
                //QUEDA DUDA
                handle_anadir_plato(result->socket, result->identificador_cliente, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
            break;
            case plato_listo:
                //FALTA TRABAJO
                handle_plato_listo(result->socket, result->identificador_cliente, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
            break;
            case confirmar_pedido:
                //FALTA TRABAJO
                handle_confirmar_pedido(result->socket, result->identificador_cliente, result->mensajes->mensajes[1]);
            break;
            case consultar_pedido:
                handle_consultar_pedido(result->socket, result->identificador_cliente, result->mensajes->mensajes[1]);
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

void handle_crear_pedido(int socket, char* id_cliente){

    char* id_pedido;

    char* respuesta[1];

    t_cliente* cliente = buscar_cliente_lista(id_cliente);

    t_restaurante* restaurante = cliente->restaurante;

    id_pedido = obtener_id_pedido(restaurante);

    respuesta[0] = id_pedido;

    enviar_mensaje_guardar_pedido(&modulo_comanda, restaurante->nombre_restaurante, respuesta[0]);

    send_messages_socket(socket, respuesta, 1);
    //liberar_conexion(socket);
}

char* obtener_id_pedido(t_restaurante* restaurante){

    t_modulo modulo_restaurante;
    modulo_restaurante.ip = NULL;
    modulo_restaurante.puerto = NULL;
    modulo_restaurante.socket = restaurante->socket;
    modulo_restaurante.identificacion = "APP";

    char* id_pedido = enviar_mensaje_crear_pedido(&modulo_restaurante);

    return id_pedido;
}

void handle_seleccionar_restaurante(int socket, char* cliente, char* restaurante){

    //TODO: Cambiar para leer los restaurantes reales
    char* respuesta[1];

    if (relacionar(restaurante, cliente) != -1){
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(socket, respuesta, 1);
    //liberar_conexion(socket);
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
    //liberar_conexion(socket);
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

void handle_consultar_platos(int socket, char* idCliente){

    List lista_platos;
    initlist(&lista_platos);

    t_modulo modulo_restaurante;
    modulo_restaurante.ip = NULL;
    modulo_restaurante.puerto = NULL;
    modulo_restaurante.socket = 0;
    modulo_restaurante.identificacion = "APP";

    t_cliente* cliente = buscar_cliente_lista(idCliente);

    t_restaurante* restaurante = cliente->restaurante;

    modulo_restaurante.socket = restaurante->socket;

    lista_platos = *enviar_mensaje_consultar_platos(&modulo_restaurante, NULL);
    
    send_message_socket(socket, obtener_platos(lista_platos));
    //liberar_conexion(socket);

}

char* obtener_platos(List lista){

    char * platos = string_new();

    string_append(&platos, "[");
    int primero = 1;
    for (IteratorList iter = beginlist(lista); iter != NULL; iter = nextlist(iter)){
        if (primero == 1){
            primero = 0;
        } else {
            string_append(&platos, ",");
        }
        char* nombrePlato = (char*) iter->data;
        string_append(&platos, nombrePlato);

    }

    string_append(&platos, "]");

    return platos;
}

void handle_anadir_plato(int socket, char* id_cliente, char* plato, char* id_pedido){

    char* respuesta[1];

    t_modulo modulo_restaurante;
    modulo_restaurante.ip = NULL;
    modulo_restaurante.puerto = NULL;
    modulo_restaurante.socket = 0;
    modulo_restaurante.identificacion = "APP";

    t_cliente* cliente = buscar_cliente_lista(id_cliente);

    t_restaurante* restaurante = cliente->restaurante;

    modulo_restaurante.socket = restaurante->socket;
    
    respuesta[0] = enviar_mensaje_anadir_plato(&modulo_restaurante, plato, id_pedido);
    if(!strcmp(respuesta[0], "FAIL")){
        send_messages_socket(socket, respuesta, 1);
        return;
    }
    
    //TODO: De donde choronga saco la cantidad?
    //respuesta[0] = enviar_mensaje_guardar_plato(&modulo_comanda, restaurante->nombre_restaurante, id_pedido, plato, /*Cantidad*/);
    if(!strcmp(respuesta[0], "FAIL")){
        send_messages_socket(socket, respuesta, 1);
        return;
    }

    send_messages_socket(socket, respuesta, 1);
    //liberar_conexion(socket);

}

void handle_plato_listo(int socket, char* restaurante, char* id_pedido, char* plato){

    char* respuesta[1];
    r_obtener_pedido* pedido;

    respuesta[0] = enviar_mensaje_plato_listo(&modulo_comanda, restaurante, id_pedido, plato);

    pedido = enviar_mensaje_obtener_pedido(&modulo_comanda, id_pedido, restaurante);

    if(comparar_platos(pedido)){
        //TODO: Logica repartidor puede retirar el pedido del restaurante
    }

    send_messages_socket(socket, respuesta, 1);
    //liberar_conexion(socket);

}

int comparar_platos(r_obtener_pedido *pedido){

    IteratorList iterador;
    informacion_comidas *info;

    if(atoi(pedido->estado) == 2){
        return 1;
    }

    for(iterador = beginlist(*pedido->info_comidas);iterador != NULL; iterador = nextlist(iterador)){
        info = (informacion_comidas*)dataiterlist(iterador);

        if(info->cantidad_lista != info->cantidad_total){
            return 0;
        }

    }

    return 1;

}

void handle_confirmar_pedido(int socket, char* id_cliente, char* id_pedido){

    char* respuesta[1];

    t_modulo modulo_restaurante;
    modulo_restaurante.ip = NULL;
    modulo_restaurante.puerto = NULL;
    modulo_restaurante.socket = 0;
    modulo_restaurante.identificacion = "APP";

    t_cliente* cliente = buscar_cliente_lista(id_cliente);

    t_restaurante* restaurante = cliente->restaurante;
    modulo_restaurante.socket = restaurante->socket;

    enviar_mensaje_obtener_pedido(&modulo_comanda, id_pedido, restaurante->nombre_restaurante);

    respuesta[0] = enviar_mensaje_confirmar_pedido(&modulo_restaurante, id_pedido, NULL);
    if(!strcmp(respuesta[0], "FAIL")){
        send_messages_socket(socket, respuesta, 1);
        return;
    }

    //TODO: Generar PCB y dejarlo en ciclo de planificacion

    respuesta[0] = enviar_mensaje_confirmar_pedido(&modulo_comanda, id_pedido, restaurante->nombre_restaurante);

    send_messages_socket(socket, respuesta, 1);
    //liberar_conexion(socket);


}

void handle_consultar_pedido(int socket, char* id_cliente, char* id_pedido){

    r_consultar_pedido pedido;
    r_obtener_pedido *pedidoAux;
    char* arrayReturn[1];

    t_cliente* cliente = buscar_cliente_lista(id_cliente);

    t_restaurante* restaurante = cliente->restaurante;

    pedido.restaurante = restaurante->nombre_restaurante;

    pedidoAux = enviar_mensaje_obtener_pedido(&modulo_comanda, id_pedido, restaurante->nombre_restaurante);

    pedido.estado = pedidoAux->estado;
    pedido.info_comidas = pedidoAux->info_comidas;

    arrayReturn[0] = armar_string_consultar_pedido(&pedido);

    send_messages_socket(socket, arrayReturn, 1);
    //liberar_conexion(socket);

}

char* armar_string_consultar_pedido(r_consultar_pedido* pedido){


    char* arrayReturn = string_new();

    string_append(&arrayReturn, "{");
    string_append(&arrayReturn, pedido->restaurante);
    string_append(&arrayReturn, ",");
    string_append(&arrayReturn, pedido->estado);
    string_append(&arrayReturn, ",");

    string_append(&arrayReturn, "[");
    for (IteratorList iter = beginlist(*pedido->info_comidas); iter != NULL; iter = nextlist(iter)){
        string_append(&arrayReturn, "{");
        informacion_comidas* info = (informacion_comidas*) iter->data;
        string_append(&arrayReturn, info->comida);
        string_append(&arrayReturn, ",");
        string_append(&arrayReturn, info->cantidad_total);
        string_append(&arrayReturn, ",");
        string_append(&arrayReturn, info->cantidad_lista);
        string_append(&arrayReturn, "}");
        string_append(&arrayReturn, ",");

    }

    string_append(&arrayReturn, "]");

    string_append(&arrayReturn, "}");

    return arrayReturn;

}