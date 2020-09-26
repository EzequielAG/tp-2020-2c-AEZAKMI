#include "api.h"

void leer_consola(t_log* logger,t_modulo* modulo) {

	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0) {
		log_info(logger, leido);
        enviar_mensajes_por_consola(modulo, leido);
		free(leido);
		leido = readline(">");
	}

	free(leido);
}

void enviar_mensajes_por_consola(t_modulo* modulo, char* mensaje_completo){

    char** string_prueba = NULL;
    string_prueba = string_split(mensaje_completo, " ");

    int numero_mensaje = obtener_numero_mensaje(string_prueba[0]);

    switch(numero_mensaje){

        case 1 : enviar_mensaje_consultar_restaurantes(modulo);
        break;
        case 2 : enviar_mensaje_seleccionar_restaurante(modulo, string_prueba[1]);
        break;
        case 3 : enviar_mensaje_obtener_restaurante(modulo,string_prueba[1]);
        break;
        case 4 : enviar_mensaje_consultar_platos(modulo,string_prueba[1]);
        break;
        case 5 : enviar_mensaje_crear_pedido(modulo);
        break;
        case 6 : enviar_mensaje_guardar_pedido(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 7 : enviar_mensaje_anadir_plato(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 8 : enviar_mensaje_guardar_plato(modulo,string_prueba[1], string_prueba[2],string_prueba[3], string_prueba[4]);
        break;
        case 9 : enviar_mensaje_confirmar_pedido(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 10 : enviar_mensaje_plato_listo(modulo,string_prueba[1], string_prueba[2],string_prueba[3]);
        break;
        case 11 : enviar_mensaje_consultar_pedido(modulo,string_prueba[1]);
        break;
        case 12 : enviar_mensaje_obtener_pedido(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 13 : enviar_mensaje_finalizar_pedido(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 14 : enviar_mensaje_terminar_pedido(modulo,string_prueba[1], string_prueba[2]);
        break;
        case 15 : enviar_mensaje_obtener_receta(modulo,string_prueba[1]);
        break;
        default : printf("NO ES UN MENSAJE VALIDO \n");

    }



}


int obtener_numero_mensaje(char* mensaje_tipo){

    if(strcmp(mensaje_tipo, "guardar_pedido") == 0){
        
        return guardar_pedido;

    } else if(strcmp(mensaje_tipo, "consultar_restaurantes") == 0){
        
        return consultar_restaurantes;

    } else if(strcmp(mensaje_tipo, "seleccionar_restaurante") == 0){
        
        return seleccionar_restaurante;

    } else if(strcmp(mensaje_tipo, "obtener_restaurante") == 0){
        
        return obtener_restaurante;

    } else if(strcmp(mensaje_tipo, "consultar_platos") == 0){
        
        return consultar_platos;

    } else if(strcmp(mensaje_tipo, "crear_pedido") == 0){
        
        return crear_pedido;

    } else if(strcmp(mensaje_tipo, "anadir_plato") == 0){
        
        return anadir_plato;

    } else if(strcmp(mensaje_tipo, "guardar_plato") == 0){
        
        return guardar_plato;

    } else if(strcmp(mensaje_tipo, "confirmar_pedido") == 0){
        
        return confirmar_pedido;

    } else if(strcmp(mensaje_tipo, "plato_listo") == 0){
        
        return plato_listo;

    } else if(strcmp(mensaje_tipo, "consultar_pedido") == 0){
        
        return consultar_pedido;

    } else if(strcmp(mensaje_tipo, "obtener_pedido") == 0){
        
        return obtener_pedido;

    } else if(strcmp(mensaje_tipo, "finalizar_pedido") == 0){
        
        return finalizar_pedido;

    } else if(strcmp(mensaje_tipo, "terminar_pedido") == 0){
        
        return terminar_pedido;

    } else if(strcmp(mensaje_tipo, "obtener_receta") == 0){
        
        return obtener_receta;

    } 
    return -1;
};



void enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }

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

    if(restaurante == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
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

    if(restaurante == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
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

    if(restaurante == NULL){
        printf("Faltan parametros \n");
        return;
    }
  
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

    if(plato == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }

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

    if(restaurante == NULL || id_pedido == NULL || comida == NULL || cantidad == NULL){
        printf("Faltan parametros \n");
        return;
    }

    char* tipo_mensaje = string_itoa(guardar_plato);
    int socket;

    char* guardar_plato[5] ={tipo_mensaje,restaurante, id_pedido, comida, cantidad};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, guardar_plato, 5);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);


    liberar_conexion(socket);
};

void enviar_mensaje_confirmar_pedido(t_modulo* modulo,char* id_pedido, char* restaurante){
   
    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
    char* tipo_mensaje = string_itoa(confirmar_pedido);
    int socket;

    char* confirmar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, confirmar_pedido, 3);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);

};

void enviar_mensaje_plato_listo(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida){

    if(restaurante == NULL || id_pedido == NULL || comida == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
    char* tipo_mensaje = string_itoa(plato_listo);
    int socket;

    char* plato_listo[4] ={tipo_mensaje, restaurante, id_pedido, comida};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, plato_listo, 4);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);
};

void enviar_mensaje_consultar_pedido(t_modulo* modulo, char* id_pedido){

    if(id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }

    char* tipo_mensaje = string_itoa(consultar_pedido);
    int socket;

    char* consultar_pedido[2] ={tipo_mensaje, id_pedido};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, consultar_pedido, 2);
    
//  t_mensajes* respuesta = receive_simple_messages(socket);

    liberar_conexion(socket);

};

void enviar_mensaje_obtener_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
    char* tipo_mensaje = string_itoa(obtener_pedido);
    int socket;

    char* obtener_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, obtener_pedido, 3);

    liberar_conexion(socket);
    
}


void enviar_mensaje_finalizar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
    char* tipo_mensaje = string_itoa(finalizar_pedido);
    int socket;

    char* finalizar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, finalizar_pedido, 3);

    liberar_conexion(socket);
    
}

void enviar_mensaje_terminar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return;
    }
    
    char* tipo_mensaje = string_itoa(terminar_pedido);
    int socket;

    char* terminar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, terminar_pedido, 3);

    liberar_conexion(socket);
    
}

void enviar_mensaje_obtener_receta(t_modulo* modulo, char* nombre_plato){

    if(nombre_plato == NULL){
        printf("Faltan parametros \n");
        return;
    }

    char* tipo_mensaje = string_itoa(obtener_receta);
    int socket;

    char* obtener_receta[2] ={tipo_mensaje, nombre_plato};
    socket = send_messages_and_return_socket(modulo->ip, modulo->puerto, obtener_receta, 2);

    liberar_conexion(socket);
}




