#include "api.h"

void enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido){
    char* tipo_mensaje = string_itoa(guardar_pedido);

    char* guardad_pedido_mensajes[3] = {tipo_mensaje, restaurante, id_pedido};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, guardad_pedido_mensajes, 3);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
     printf("\n");

    liberar_conexion(socket);
}


void enviar_mensaje_consultar_restaurantes(t_modulo* modulo){
    
    char* tipo_mensaje = string_itoa(consultar_restaurantes);
    char* get_restaurantes[1] = {tipo_mensaje};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, get_restaurantes, 1);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
        printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");

    liberar_conexion(socket);
}

void enviar_mensaje_seleccionar_restaurante(t_modulo* modulo, char* restaurante){
    
    char* tipo_mensaje = string_itoa(seleccionar_restaurante);

    //TODO: Definir como se elige el ID del cliente
    char* seleccionar_restaurantes[3] = {tipo_mensaje, restaurante, "Cliente 1"};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, seleccionar_restaurantes, 3);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
        printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");

    liberar_conexion(socket);
}

void enviar_mensaje_crear_pedido(t_modulo* modulo){

    char* tipo_mensaje = string_itoa(crear_pedido);
    char* crear_pedido_mensajes[1] = {tipo_mensaje};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, crear_pedido_mensajes, 1);

    t_mensajes* respuesta = receive_simple_messages(socket);

    char* id_pedido = respuesta->mensajes[0];
    printf("%s\n", id_pedido);

    liberar_conexion(socket);
}

void enviar_mensaje_obtener_restaurante(t_modulo* modulo, char* restaurante){
    
    char* tipo_mensaje = string_itoa(obtener_restaurante);
    char* obtener_restaurante[2] ={tipo_mensaje,restaurante};
    int socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, obtener_restaurante, 2);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
     printf("\n");

    liberar_conexion(socket);

}

void enviar_mensaje_consultar_platos(t_modulo* modulo, char* restaurante){
  
    char* tipo_mensaje = string_itoa(consultar_platos);
    int socket ;

    if(!strcmp((modulo->nombre),"sindicato")){
        char* consulta_platos[2] ={tipo_mensaje,restaurante};
        socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, consulta_platos, 2);
    
    }else{
        socket = send_message_and_return_socket(modulo->ip, modulo->puerto, tipo_mensaje);
    }

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");
    
    liberar_conexion(socket);

}

void enviar_mensaje_anadir_plato(t_modulo* modulo, char* plato, char* id_pedido){

    char* tipo_mensaje = string_itoa(anadir_plato);
    int socket;

    char* anadir_plato[3] ={tipo_mensaje,plato, id_pedido};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, anadir_plato, 3);
    
    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");
    
    liberar_conexion(socket);
};

void enviar_mensaje_guardar_plato(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida, char* cantidad){

    char* tipo_mensaje = string_itoa(guardar_plato);
    int socket;

    char* guardar_plato[5] ={tipo_mensaje,restaurante, id_pedido, comida, cantidad};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, guardar_plato, 5);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);


    liberar_conexion(socket);
};

void enviar_mensaje_confirmar_pedido(t_modulo* modulo,char* id_pedido, char* restaurante){
    
    char* tipo_mensaje = string_itoa(confirmar_pedido);
    int socket;

    char* confirmar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, confirmar_pedido, 3);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);

};

void enviar_mensaje_plato_listo(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida){
    
    char* tipo_mensaje = string_itoa(plato_listo);
    int socket;

    char* plato_listo[4] ={tipo_mensaje, restaurante, id_pedido, comida};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, plato_listo, 4);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);
};

void enviar_mensaje_consultar_pedido(t_modulo* modulo, char* id_pedido){

    char* tipo_mensaje = string_itoa(consultar_pedido);
    int socket;

    char* consultar_pedido[2] ={tipo_mensaje, id_pedido};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, consultar_pedido, 2);
    
//  t_mensajes* respuesta = receive_simple_messages(socket);

    liberar_conexion(socket);

};

void enviar_mensaje_obtener_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){
    
    char* tipo_mensaje = string_itoa(obtener_pedido);
    int socket;

    char* obtener_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, obtener_pedido, 3);

    liberar_conexion(socket);
    
}


void enviar_mensaje_finalizar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){
    
    char* tipo_mensaje = string_itoa(finalizar_pedido);
    int socket;

    char* finalizar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, finalizar_pedido, 3);

    liberar_conexion(socket);
    
}

void enviar_mensaje_terminar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){
    
    char* tipo_mensaje = string_itoa(terminar_pedido);
    int socket;

    char* terminar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, terminar_pedido, 3);

    liberar_conexion(socket);
    
}

void enviar_mensaje_obtener_receta(t_modulo* modulo, char* nombre_plato){

    char* tipo_mensaje = string_itoa(obtener_receta);
    int socket;

    char* obtener_receta[2] ={tipo_mensaje, nombre_plato};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, obtener_receta, 2);

    liberar_conexion(socket);
}




