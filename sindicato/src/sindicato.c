#include "sindicato.h"

int main(void){
    sindicato_init(&sindicato_config, &logger);
    get_or_create_fs();

    printf("Imprimiendo el path %s", sindicato_config->ruta_log);

    //TODO: Hacer que reciba ip y puerto de config
    iniciar_servidor("127.0.0.1", "5003", handle_client);

    sindicato_finally(sindicato_config, logger);
    return 0;
}

void handle_client(t_result* result){
    
    if (result->operacion == MENSAJE){
        if (!strcmp(result->mensaje, "HANDSHAKE")){
            send_message_socket(result->socket, "OK");
            liberar_conexion(result->socket);
        }
    } else {
        if (result->operacion == MENSAJES){
            int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
            switch(tipo_mensaje)
            {
                case consultar_platos:
                    handle_consultar_platos(result->socket, result->mensajes->mensajes[1]);
                    break;
                case guardar_pedido:
                    handle_guardar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case guardar_plato:
                    handle_guardar_plato(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3], result->mensajes->mensajes[4]);
                    break;
                case confirmar_pedido:
                    handle_confirmar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case obtener_pedido:
                    handle_obtener_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                case obtener_restaurante:
                    handle_obtener_restaurante(result->socket, result->mensajes->mensajes[1]);
                    break;
                case plato_listo:
                    handle_plato_listo(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2], result->mensajes->mensajes[3]);
                    break;
                case obtener_receta:
                    handle_obtener_receta(result->socket, result->mensajes->mensajes[1]);
                    break;
                case terminar_pedido:
                    handle_terminar_pedido(result->socket, result->mensajes->mensajes[1], result->mensajes->mensajes[2]);
                    break;
                default:
                    handle_error(result->socket);
                    break;
            }
    
        }
    }    

    liberar_conexion(result->socket);
    return;
}

int guardar_pedido_en_afip(char* restaurante, char* id_pedido){
    //TODO: Guardar pedido en afip, 1 OK 0 FAIL
    
    return 1;
}

void handle_consultar_platos(int socket, char* restaurante){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Obtener los platos que puede preparar dicho Restaurante del archivo info.AFIP.

    //Responder el mensaje indicando los platos que puede preparar el Restaurante.

}

void handle_guardar_pedido(int socket, char* restaurante, char* id_pedido){
    
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar que el ID de pedido no exista para dicho restaurante. 
    //En caso de existir se deberá informar sobre dicha situación. 
    //En caso de que no exista, se deberá crear el archivo.  
    if (resultado_operacion){
        resultado_operacion = guardar_pedido_en_afip(restaurante, id_pedido);
    }

    //Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
    char* respuesta[1];

    if (resultado_operacion){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }
    send_messages_socket(socket, respuesta, 1);
}

void handle_guardar_plato(int socket, char* restaurante, char* id_pedido, char* comida, char* cantidad){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar si el Pedido existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
    //En caso de no existir se deberá informar dicha situación.

    //Verificar que el pedido esté en estado “Pendiente”. En caso contrario se deberá informar dicha situación.

    //Verificar si ese plato ya existe dentro del archivo. 
    //En caso de existir, se deberán agregar la cantidad pasada por parámetro a la actual. 
    //En caso de no existir se deberá agregar el plato a la lista de Platos y anexar la cantidad que se tiene que cocinar de dicho plato y aumentar el precio total del pedido.

    //Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
    char* respuesta[1];

    if (resultado_operacion){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }
    send_messages_socket(socket, respuesta, 1);

}

void handle_confirmar_pedido(int socket, char* id_pedido,  char* restaurante){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar si el Pedido existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
    //En caso de no existir se deberá informar dicha situación.

    //Verificar que el pedido esté en estado “Pendiente”. En caso contrario se deberá informar dicha situación.

    //Cambiar el estado del pedido de “Pendiente” a “Confirmado” (se debe truncar el archivo en caso de ser necesario).

    //Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
    char* respuesta[1];

    if (resultado_operacion){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }
    send_messages_socket(socket, respuesta, 1);
}

void handle_obtener_pedido(int socket, char* restaurante,  char* id_pedido){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar si el Pedido existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
    //En caso de no existir se deberá informar dicha situación.

    //Responder el mensaje indicando si se pudo realizar en conjunto con la información del pedido si correspondiera.

}

void handle_obtener_restaurante(int socket, char* restaurante){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Obtener todo los datos del archivo info.AFIP.

    //Responder el mensaje indicando los datos del Restaurante.

}

void handle_plato_listo(int socket, char* restaurante,  char* id_pedido, char* comida){

    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar si el Pedido existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
    //En caso de no existir se deberá informar dicha situación.

    //Verificar que el pedido esté en estado “Confirmado”. 
    //En caso contrario se deberá informar dicha situación.

    //Verificar si ese plato ya existe dentro del archivo. 
    //En caso de existir, se deberá aumentar en uno la cantidad lista de ese plato. 
    //En caso contrario se deberá informar dicha situación.

    //Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
    char* respuesta[1];

    if (resultado_operacion){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }
    send_messages_socket(socket, respuesta, 1);

}

void handle_obtener_receta(int socket, char* comida){
    //Verificar si existe el plato dado dentro del directorio de recetas.
    //En caso de no existir, se deberá informar dicha situación.

    //Responder el mensaje con la receta solicitada.

}

void handle_terminar_pedido(int socket, char* id_pedido,  char* restaurante){
    //Verificar si el Restaurante existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio Restaurantes si existe un subdirectorio con el nombre del Restaurante. 
    //En caso de no existir se deberá informar dicha situación.
    int resultado_operacion = existe_restaurante(restaurante);

    //Verificar si el Pedido existe dentro del File System. 
    //Para esto se deberá buscar dentro del directorio del Restaurante si existe dicho pedido. 
    //En caso de no existir se deberá informar dicha situación.

    //Verificar que el pedido esté en estado “Confirmado”. En caso contrario se deberá informar dicha situación.

    //Cambiar el estado del pedido a “Terminado” 

    //Responder el mensaje indicando si se pudo realizar la operación correctamente (Ok/Fail).
    char* respuesta[1];

    if (resultado_operacion){
        respuesta[0] = "Ok";
    } else {
        respuesta[0] = "Fail";
    }
    send_messages_socket(socket, respuesta, 1);

}

void handle_error(int socket){
    char* respuesta[1];
    respuesta[0] = "ERROR";
    send_messages_socket(socket, respuesta, 1);
}