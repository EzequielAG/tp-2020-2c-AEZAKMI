#include "pedidos_mensajes.h"

void handle_client(t_result* result){

    //CADA UNO DE LOS MENSAJES CON UN HILO DISTINTO; PASAR POR PARAMETRO SOLAMENTE EL T_RESULT

    l_pedidoRecibido *pedidos = malloc(sizeof(l_pedidoRecibido));
    pedidos->socket = result->socket;
    pedidos->nombreResto = result->mensajes->mensajes[1];
    pedidos->idPedido = result->mensajes->mensajes[2];
    pedidos->plato = result->mensajes->mensajes[3];
    pedidos->cantidadPlato = atoi(result->mensajes->mensajes[4]);


    if (result->operacion == MENSAJE){
        if (!strcmp(result->mensaje, "HANDSHAKE")){
            send_message_socket(result->socket, "OK");
            liberar_conexion(result->socket);
        }
    } else {
        if (result->operacion == MENSAJES){
            int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
            if (tipo_mensaje == guardar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO
                handle_guardar_pedidos(pedidos);
            } else if (tipo_mensaje == guardar_plato){ // NOMBRE_RESTAURANTE ID_PEDIDO PLATO CANTIDAD_PLATO
                handle_guardar_plato(pedidos);
            } else if (tipo_mensaje == confirmar_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO 
                handle_confirmar_pedido(pedidos);
            } else if (tipo_mensaje == plato_listo){ //  NOMBRE_RESTAURANTE ID_PEDIDO PLATO
                handle_plato_listo(pedidos);
            } else if (tipo_mensaje == obtener_pedido){ // NOMBRE_RESTAURANTE ID_PEDIDO
                handle_obtener_pedido(pedidos);
            } else if (tipo_mensaje == finalizar_pedido){ //  NOMBRE_RESTAURANTE ID_PEDIDO
                handle_finalizar_pedido(pedidos);
            }else if (tipo_mensaje == handshake_cliente){

                printf("Se conecto el cliente con el id: %s", result->mensajes->mensajes[1]);
            }
 
        }
    }    

    free(pedidos);

    return;
}


void handle_guardar_pedidos(l_pedidoRecibido *pedidos){
    
    char* respuesta[1];

    if (guardar_pedido_en_memoria(pedidos->nombreResto, pedidos->idPedido)){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }

    send_messages_socket(pedidos->socket, respuesta, 1);
    liberar_conexion(pedidos->socket);
}

int guardar_pedido_en_memoria(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en memoria, 1 OK 0 FAIL
    
    return 1;
}

void handle_guardar_plato(l_pedidoRecibido *pedidos){

}

void handle_confirmar_pedido(l_pedidoRecibido *pedidos){

}

void handle_plato_listo(l_pedidoRecibido *pedidos){

}

void handle_obtener_pedido(l_pedidoRecibido *pedidos){

}

void handle_finalizar_pedido(l_pedidoRecibido *pedidos){

}
