#include "pedidos_mensajes.h"

void handle_client(t_result* result){

    //CADA UNO DE LOS MENSAJES CON UN HILO DISTINTO; PASAR POR PARAMETRO SOLAMENTE EL T_RESULT

  


    if (result->operacion == MENSAJE){
        if (!strcmp(result->mensaje, "HANDSHAKE")){
            send_message_socket(result->socket, "OK");
            liberar_conexion(result->socket);
        }
    } else {
        if (result->operacion == MENSAJES){
            int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
            if (tipo_mensaje == guardar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO
                handle_guardar_pedidos(result);
            } else if (tipo_mensaje == guardar_plato){ // NOMBRE_RESTAURANTE ID_PEDIDO PLATO CANTIDAD_PLATO
                handle_guardar_plato(result);
            } else if (tipo_mensaje == confirmar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO 
                handle_confirmar_pedido(result);
            } else if (tipo_mensaje == plato_listo){ //  NOMBRE_RESTAURANTE ID_PEDIDO PLATO
                handle_plato_listo(result);
            } else if (tipo_mensaje == obtener_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO
                handle_obtener_pedido(result);
            } else if (tipo_mensaje == finalizar_pedido){ //  NOMBRE_RESTAURANTE ID_PEDIDO
                handle_finalizar_pedido(result);
            }else if (tipo_mensaje == handshake_cliente){

                printf("Se conecto el cliente con el id: %s", result->mensajes->mensajes[1]);
            }
 
        }
    }    

    return;
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

int guardar_pedido_en_memoria(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en memoria, 1 OK 0 FAIL
    
    return 1;
}

void handle_guardar_plato(t_result* result){

}

void handle_confirmar_pedido(t_result* result){

}

void handle_plato_listo(t_result* result){

}

void handle_obtener_pedido(t_result* result){

}

void handle_finalizar_pedido(t_result* result){

}
