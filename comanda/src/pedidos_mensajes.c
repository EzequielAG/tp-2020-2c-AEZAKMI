#include "pedidos_mensajes.h"

void handle_client(t_result* result){

    //CADA UNO DE LOS MENSAJES CON UN HILO DISTINTO; PASAR POR PARAMETRO SOLAMENTE EL T_RESULT

    pthread_t guardar_pedido_thread;
    pthread_t guardar_plato_thread;
    pthread_t confirmar_pedido_thread;
    pthread_t plato_listo_thread;
    pthread_t obtener_pedido_thread;
    pthread_t finalizar_pedido_thread;
   
    IteratorList iterador = NULL;

    l_proceso *resto = NULL;


    printf("La tabla de restaurante contiene los siguientes datos \n");
    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);

        printf("Restaurante: %s | Direccion %p \n", resto->nombreResto,resto->punteroTablaSegmentos);
    }printf("------------- \n");



    int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
    if (tipo_mensaje == guardar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO

        pthread_create(&guardar_pedido_thread, NULL, (void*)handle_guardar_pedidos, (void*)result);
	    pthread_join(guardar_pedido_thread, NULL);

    } else if (tipo_mensaje == guardar_plato){ // NOMBRE_RESTAURANTE ID_PEDIDO PLATO CANTIDAD_PLATO

        pthread_create(&guardar_plato_thread, NULL, (void*)handle_guardar_plato, result);
	    pthread_join(guardar_plato_thread, NULL);

    } else if (tipo_mensaje == confirmar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO 

        pthread_create(&confirmar_pedido_thread, NULL, (void*)handle_confirmar_pedido, result);
	    pthread_join(confirmar_pedido_thread, NULL);

    } else if (tipo_mensaje == plato_listo){ //  NOMBRE_RESTAURANTE ID_PEDIDO PLATO

        pthread_create(&plato_listo_thread, NULL, (void*)handle_plato_listo, result);
	    pthread_join(plato_listo_thread, NULL);

    } else if (tipo_mensaje == obtener_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO

        pthread_create(&obtener_pedido_thread, NULL, (void*)handle_obtener_pedido, result);
	    pthread_join(obtener_pedido_thread, NULL);

    } else if (tipo_mensaje == finalizar_pedido){ //  NOMBRE_RESTAURANTE ID_PEDIDO

        pthread_create(&finalizar_pedido_thread, NULL, (void*)handle_finalizar_pedido, result);
	    pthread_join(finalizar_pedido_thread, NULL);


    }else if (tipo_mensaje == handshake_cliente){
        send_message_socket(result->socket, "OK");
        liberar_conexion(result->socket);
        printf("Se conecto el cliente con el id: %s \n", result->mensajes->mensajes[1]);
    }
 


    return;
}

int existe_restaurante(char* restaurante){
    IteratorList iterador = NULL;
    l_proceso* resto = NULL;

    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);

        if(!strcmp(resto->nombreResto,restaurante)){
            return 1;
        }
    }       

    return 0;

}

//ESTOS DOS QUEDAN ASI PARA TESTEAR

void handle_guardar_plato(t_result* result){

    char* respuesta[1];

    if (guardar_plato_en_memoria(result->mensajes->mensajes[1],result->mensajes->mensajes[2],result->mensajes->mensajes[4],result->mensajes->mensajes[3])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}


void handle_guardar_pedidos(t_result* result){
    
    char* respuesta[1];

    if (guardar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}




void handle_confirmar_pedido(t_result* result){

    char* respuesta[1];

    if (confirmar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}

void handle_plato_listo(t_result* result){

    char* respuesta[1];

    if (plato_listo_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}

//COORDINAR CON APP PARA EL FORMATE DE RESPUESTA
void handle_obtener_pedido(t_result* result){

    char* respuesta[1];

    if (obtener_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}

void handle_finalizar_pedido(t_result* result){

    char* respuesta[1];

    if (finalizar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}


int guardar_plato_en_memoria(char* nombreResto, char* idPedido, char* cantidadPlato, char* plato){
    
    l_proceso *restoEnTabla = find_resto_lista(nombreResto);

    if(restoEnTabla == NULL){
        
        printf("El restaurante no esta en la tabla \n");
        
        return 0;
    }

    //DEVUELVE NULL Y PRODUCE EL SEGMENTATION FAULT

    l_segmento *segmento = find_segmento_lista(idPedido, restoEnTabla->punteroTablaSegmentos);

    crear_pagina(segmento, atoi(cantidadPlato), plato);

    return 1;
};


int guardar_pedido_en_memoria(char* restaurante, char* id_pedido){


    l_proceso *restoEnTabla = find_resto_lista(restaurante);

    if(restoEnTabla == NULL){
        pushbacklist(&tablaRestaurantes, (void *) crearProceso(restaurante));
        restoEnTabla = backlist(tablaRestaurantes);
    }

    crearSegmento(restoEnTabla, id_pedido);

    return 1;
}

int confirmar_pedido_en_memoria(char* restaurante, char* id_pedido){



    return 1;
}

int obtener_pedido_en_memoria(char* restaurante, char* id_pedido){
    return 1;
}

int plato_listo_en_memoria(char* restaurante, char* id_pedido, char* plato){



    return 1;
}

int finalizar_pedido_en_memoria(char* restaurante, char* id_pedido){
    return 1;
}