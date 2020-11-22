#include "pedidos_mensajes.h"

void handle_client(t_result* result){

    //CADA UNO DE LOS MENSAJES CON UN HILO DISTINTO; PASAR POR PARAMETRO SOLAMENTE EL T_RESULT

    pthread_t guardar_pedido_thread;
    pthread_t guardar_plato_thread;
    pthread_t confirmar_pedido_thread;
    pthread_t plato_listo_thread;
    pthread_t obtener_pedido_thread;
    pthread_t finalizar_pedido_thread;
    

    int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
    if (tipo_mensaje == guardar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO

        pthread_create(&guardar_pedido_thread, NULL, (void*)handle_guardar_pedidos, (void*)result);
	    pthread_join(guardar_pedido_thread, NULL); //MENSAJE LISTO EN TEORIA XD

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
    }else if (tipo_mensaje == 20){
        printf("Llego el mensaje CREAR_MENSAJE\n");
        send_message_socket(result->socket, "3");
        liberar_conexion(result->socket);
    }
        /*
    else if (!strcmp(result->identificador_cliente, "APP")){
        send_message_socket(result->socket, "OK");
        liberar_conexion(result->socket);
        printf("Se conecto el cliente con el id: %s \n", result->mensajes->mensajes[1]);
    }
    */


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
        imprimirTodo();
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}


void handle_guardar_pedidos(t_result* result){
    
    char* respuesta[1];

    if (guardar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        imprimirTodo();
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);
}




void handle_confirmar_pedido(t_result* result){

    char* respuesta[1];

    if (confirmar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        imprimirTodo();
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}

void handle_plato_listo(t_result* result){

    char* respuesta[1];

    if (plato_listo_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3])){
        imprimirTodo();
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}

void handle_obtener_pedido(t_result* result){

    IteratorList iterator = NULL;

    l_segmento* segmento = obtener_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2]);

    if(segmento != NULL){

        char* arrayReturn[4];
        strcpy(arrayReturn[0], string_itoa(segmento->estadoPedido));

        for(iterator = beginlist(*segmento->punteroTablaPaginas); iterator != NULL; iterator = nextlist(iterator)){
            l_pagina* pagina = (l_pagina*) dataiterlist(iterator);
            l_frame* frame = pagina->swap;

            char* cantidad = string_itoa(frame->cantidadPlato);
            char* cantidadLista = string_itoa(frame->cantidadLista);

            string_append(&arrayReturn[1], frame->plato);
            string_append(&arrayReturn[1], ",");
            string_append(&arrayReturn[2], cantidad);
            string_append(&arrayReturn[2], ",");
            string_append(&arrayReturn[3], cantidadLista);
            string_append(&arrayReturn[3], ",");    

            free(cantidad);
            free(cantidadLista);

        }
        
        send_messages_socket(result->socket, arrayReturn, 4);
    }else{
        char* arrayReturn[1];
        arrayReturn[0] = "FAIL";
        send_messages_socket(result->socket, arrayReturn, 1);
    }

    liberar_conexion(result->socket);
   
}

void handle_finalizar_pedido(t_result* result){

    char* respuesta[1];

    if (finalizar_pedido_en_memoria(result->mensajes->mensajes[1], result->mensajes->mensajes[2])){
        imprimirTodo();
        respuesta[0] = "OK";
    } else {
        respuesta[0] = "FAIL";
    }

    send_messages_socket(result->socket, respuesta, 1);
    liberar_conexion(result->socket);

}


int guardar_plato_en_memoria(char* nombreResto, char* idPedido, char* cantidadPlato, char* plato){
    
    l_proceso *restoEnTabla = find_resto_lista(nombreResto);

    if(restoEnTabla == NULL){
        
        printf("El restaurante no esta en la tabla de restaurantes \n");
        
        return 0;
    }

    l_segmento *segmento = find_segmento_lista(idPedido, restoEnTabla->punteroTablaSegmentos);

    if(segmento == NULL){

        printf("El segmento no esta en la tabla de segmentos \n");
        
        return 0;

    }

    l_pagina* pagina_plato = plato_en_pagina(plato, segmento->punteroTablaPaginas);

    if(pagina_plato == NULL){
        crear_pagina2(segmento, atoi(cantidadPlato), plato); 
        return 1; 
    }

    modificarPagina(pagina_plato);

    agregar_plato_pedido(pagina_plato,atoi(cantidadPlato));

    return 1;
};


int guardar_pedido_en_memoria(char* restaurante, char* id_pedido){


    l_proceso *restoEnTabla = find_resto_lista(restaurante);

    if(restoEnTabla == NULL){
        pushbacklist(&tablaRestaurantes, (void *) crearProceso(restaurante));
        restoEnTabla = backlist(tablaRestaurantes);
    }

    return crearSegmento(restoEnTabla, id_pedido);      

}

int confirmar_pedido_en_memoria(char* restaurante, char* id_pedido){

    l_segmento* segmento = obtener_pedido_en_memoria(restaurante, id_pedido);

    if(segmento == NULL){
        return 0;
    }

    confirmar_pedido_segmento(segmento);

    return 1;
}



l_segmento* obtener_pedido_en_memoria(char* nombreResto, char* id_pedido){

    l_proceso *restoEnTabla = find_resto_lista(nombreResto);

    if(restoEnTabla == NULL){
        
        printf("El restaurante no esta en la tabla de restaurantes \n");
        
        return NULL;
    }

    l_segmento *segmento = find_segmento_lista(id_pedido, restoEnTabla->punteroTablaSegmentos);

    pasarPaginasAPrincipal(segmento);

    return segmento;
}

int plato_listo_en_memoria(char* nombreResto, char* idPedido, char* plato){
 
    l_proceso *restoEnTabla = find_resto_lista(nombreResto);

    if(restoEnTabla == NULL){
        
        printf("El restaurante no esta en la tabla de restaurantes \n");
        
        return 0;
    }

    l_segmento *segmento = find_segmento_lista(idPedido, restoEnTabla->punteroTablaSegmentos);

    if(segmento == NULL){

        printf("El segmento no esta en la tabla de segmentos \n");
        
        return 0;

    }

    if(segmento->estadoPedido == 0 || segmento->estadoPedido == 2){

        printf("El pedido no esta confirmado o ya esta terminado \n");
        
        return 0;

    }

    l_pagina* pagina_plato = plato_en_pagina(plato, segmento->punteroTablaPaginas);

    modificarPagina(pagina_plato);

    terminarPlatoPagina(pagina_plato);
    /*
    if(platos_listos(segmento)){
        terminar_pedido_segmento(segmento); 
    };
    */
    return 1;
}

int finalizar_pedido_en_memoria(char* restaurante, char* id_pedido){

    l_proceso *restoEnTabla = find_resto_lista(restaurante);

    if(restoEnTabla == NULL){
        
        printf("El restaurante no esta en la tabla de restaurantes \n");
        
        return 0;
    }

    l_segmento *segmento = find_segmento_lista(id_pedido, restoEnTabla->punteroTablaSegmentos);

    if(segmento == NULL){

        printf("El segmento no esta en la tabla de segmentos \n");
        
        return 0;

    }

    eliminarSegmento(restoEnTabla, segmento);

    return 1;
}
