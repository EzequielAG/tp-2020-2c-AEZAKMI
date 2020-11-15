#include "api.h"


char* enviar_mensaje_guardar_pedido(t_modulo* modulo, char* restaurante, char* id_pedido){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(guardar_pedido);

    char* guardad_pedido_mensajes[3] = {tipo_mensaje, restaurante, id_pedido};
    int socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, guardad_pedido_mensajes, 3);

    if(socket == -1){
        return "FAIL";
    }
    
    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
     printf("\n");

    liberar_conexion(socket);

    return respuesta->mensajes[0];
}


char** enviar_mensaje_consultar_restaurantes(t_modulo* modulo){
    
    char* tipo_mensaje = string_itoa(consultar_restaurantes);
    char* get_restaurantes[1] = {tipo_mensaje};
    int socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, get_restaurantes, 1);

    t_mensajes* respuesta = receive_simple_messages(socket);

    char** respuesta_restaurantes = malloc(sizeof(char*) * *respuesta->size);

    for (int i= 0; i < *respuesta->size; i++){
        printf("%s ", respuesta->mensajes[i]);
        respuesta_restaurantes [i] = respuesta->mensajes[i];
    }
    printf("\n");

    liberar_conexion(socket);

    return respuesta_restaurantes;

}

char* enviar_mensaje_seleccionar_restaurante(t_modulo* modulo, char* id_cliente, char* nombre_restaurante){

    if(nombre_restaurante == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(seleccionar_restaurante);

    //TODO: Definir como se elige el ID del cliente
    char* seleccionar_restaurantes[3] = {tipo_mensaje, id_cliente, nombre_restaurante};
    int socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, seleccionar_restaurantes, 3);

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
        printf(" %s ", respuesta->mensajes[i]);
    }
    printf("\n");

    char* respuesta_seleccion_restaurante = respuesta->mensajes[0];

    liberar_conexion(socket);

    return respuesta_seleccion_restaurante;
}

char* enviar_mensaje_crear_pedido(t_modulo* modulo){

    char* tipo_mensaje = string_itoa(crear_pedido);
    char* crear_pedido_mensajes[1] = {tipo_mensaje};
    int socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, crear_pedido_mensajes, 1);

    t_mensajes* respuesta = receive_simple_messages(socket);

    char* id_pedido = respuesta->mensajes[0];

    printf("%s\n", id_pedido);

    liberar_conexion(socket);

    return id_pedido;
}

r_obtener_restaurante* enviar_mensaje_obtener_restaurante(t_modulo* modulo, char* restaurante){

    if(restaurante == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(obtener_restaurante);
    char* obtener_restaurante[2] ={tipo_mensaje,restaurante};
    int socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, obtener_restaurante, 2);

    t_mensajes* respuesta = receive_simple_messages(socket);

    r_obtener_restaurante* respuesta_obtener_restaurante = malloc (sizeof(r_obtener_restaurante));

    respuesta_obtener_restaurante->afinidades = obtener_list_mensajes(respuesta->mensajes[0]);;
    respuesta_obtener_restaurante->pos_x = respuesta->mensajes[1];
    respuesta_obtener_restaurante->pos_y = respuesta->mensajes[2];
    respuesta_obtener_restaurante->recetas_precio = obtener_receta_precios(respuesta->mensajes[3]) ;
    respuesta_obtener_restaurante->cantidad_hornos = respuesta->mensajes[4];
    respuesta_obtener_restaurante->cantidad_pedidos = respuesta->mensajes[5];
    respuesta_obtener_restaurante->cantidad_cocineros = respuesta->mensajes[6];

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
     printf("\n");

    liberar_conexion(socket);

    return respuesta_obtener_restaurante;

}

char** obtener_array_mensajes(char* array_mensaje){

    char** array_string = string_split(array_mensaje, ",");
    array_string =separar_por_comillas(array_string);
 
    return array_string;

}

List* obtener_list_mensajes(char* array_mensaje){

    char** array_string = string_split(array_mensaje, ",");
    List* resultado = separar_por_comillas_lista(array_string);

    return resultado;
}

receta_precio** obtener_receta_precios(char* array_mensajes){

    char** array_string = string_split(array_mensajes, "|");

    receta_precio** receta_precio_final = malloc ( sizeof(receta_precio) * sizeof(receta_precio) );

    for(int i = 0; array_string[i]!=NULL; i++){
        char** receta_precio_individual = string_split(array_string[i], ",");

        receta_precio_final[i]->receta = receta_precio_individual[0];
        receta_precio_final[i]->precio = receta_precio_individual[1];

    }

    return receta_precio_final;
}


List* enviar_mensaje_consultar_platos(t_modulo* modulo, char* restaurante){

    if(restaurante == NULL){
        printf("Faltan parametros \n");
        return NULL;;
    }
  
    char* tipo_mensaje = string_itoa(consultar_platos);
    int socket ;

    if(!strcmp((modulo->identificacion),"sindicato")){
        char* consulta_platos[2] ={tipo_mensaje,restaurante};
        socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, consulta_platos, 2);
    }else{
        socket = send_message_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, tipo_mensaje);
    }

     if(socket == -1){
        return NULL;
    }

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");
    
    liberar_conexion(socket);

    return obtener_list_mensajes(respuesta->mensajes[0]);
}

char* enviar_mensaje_anadir_plato(t_modulo* modulo, char* plato, char* id_pedido){

    if(plato == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(anadir_plato);
    int socket;

    char* anadir_plato[3] ={tipo_mensaje,plato, id_pedido};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, anadir_plato, 3);
    
     if(socket == -1){
        return "FAIL";
    }

    t_mensajes* respuesta = receive_simple_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    }
    printf("\n");
    
    liberar_conexion(socket);

    return respuesta->mensajes[0];
};

char* enviar_mensaje_guardar_plato(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida, char* cantidad){

    if(restaurante == NULL || id_pedido == NULL || comida == NULL || cantidad == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(guardar_plato);
    int socket;

    char* guardar_plato[5] ={tipo_mensaje,restaurante, id_pedido, comida, cantidad};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, guardar_plato, 5);
    
    if(socket == -1){
        return "FAIL";
    }
   t_mensajes* respuesta = receive_simple_messages(socket);

   printf("%s \n", respuesta->mensajes[0]);


    liberar_conexion(socket);

    return respuesta->mensajes[0];
};

char* enviar_mensaje_confirmar_pedido(t_modulo* modulo,char* id_pedido, char* restaurante){
   
    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(confirmar_pedido);
    int socket;

    char* confirmar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, confirmar_pedido, 3);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);

    return respuesta->mensajes[0];

};

char* enviar_mensaje_plato_listo(t_modulo* modulo, char* restaurante, char* id_pedido, char* comida){

    if(restaurante == NULL || id_pedido == NULL || comida == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(plato_listo);
    int socket;

    char* plato_listo[4] ={tipo_mensaje, restaurante, id_pedido, comida};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, plato_listo, 4);
    
   t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n", respuesta->mensajes[0]);

    liberar_conexion(socket);

    return respuesta->mensajes[0];
};

r_consultar_pedido* enviar_mensaje_consultar_pedido(t_modulo* modulo, char* id_pedido){

    if(id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(consultar_pedido);
    int socket;

    char* consultar_pedido[2] ={tipo_mensaje, id_pedido};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, consultar_pedido, 2);
    
    t_mensajes* respuesta = receive_messages(socket);

    r_consultar_pedido* respuesta_consulta_pedido = malloc(sizeof(r_consultar_pedido));

    respuesta_consulta_pedido->restaurante = respuesta->mensajes[0];
    respuesta_consulta_pedido->estado = respuesta->mensajes[1];
    respuesta_consulta_pedido->info_comidas = obtener_informacion_comidas(respuesta->mensajes[2],respuesta->mensajes[3],respuesta->mensajes[4]);


    for (int i= 0; i < *respuesta->size; i++){
       printf("%s ", respuesta->mensajes[i]);
    } printf("\n");

    liberar_conexion(socket);

    return NULL;

};


List* obtener_informacion_comidas(char* platos,char* cantidades_listas, char* cantidades_totales){

    List* lista_comidas = malloc(sizeof(List));

    initlist(lista_comidas);

    char** mensajes = string_split(platos, ",");
    char** listas = string_split(cantidades_listas, ",");
    char** totales = string_split(cantidades_totales, ",");
    
    for(int i = 0; mensajes[i]!=NULL; i++){

        informacion_comidas* info_comida = malloc(sizeof(informacion_comidas));
        info_comida->comida = mensajes[i];
        info_comida->cantidad_lista = listas[i];
        info_comida->cantidad_total = totales[i];

        pushbacklist(lista_comidas,info_comida);

    }

    return lista_comidas;

};

r_obtener_pedido* enviar_mensaje_obtener_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){
    
    if(restaurante == NULL || id_pedido == NULL){
            printf("Faltan parametros \n");
            return NULL;
        }

    char* tipo_mensaje = string_itoa(obtener_pedido);

    int socket = 0;

    char* obtener_pedido[3] ={tipo_mensaje, id_pedido, restaurante};

    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, obtener_pedido, 3);

    t_mensajes* respuesta = receive_messages(socket);

    for (int i= 0; i < *respuesta->size; i++){
        printf("%s ", respuesta->mensajes[i]);
    } printf("\n");

    r_obtener_pedido* respuesta_obtener_pedido = malloc(sizeof(r_obtener_pedido));

    respuesta_obtener_pedido->estado = respuesta->mensajes[0];
    respuesta_obtener_pedido->info_comidas = obtener_informacion_comidas(respuesta->mensajes[1],respuesta->mensajes[2], respuesta->mensajes[3]);

    liberar_conexion(socket);

    return respuesta_obtener_pedido;
    
}



char* enviar_mensaje_finalizar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(finalizar_pedido);
    int socket;

    char* finalizar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, finalizar_pedido, 3);

    t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n" ,respuesta->mensajes[0]);

    liberar_conexion(socket);
    
    return respuesta->mensajes[0];
}

char* enviar_mensaje_terminar_pedido(t_modulo* modulo, char* id_pedido,char* restaurante){

    if(restaurante == NULL || id_pedido == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }
    
    char* tipo_mensaje = string_itoa(terminar_pedido);
    int socket;

    char* terminar_pedido[3] ={tipo_mensaje, id_pedido, restaurante};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, terminar_pedido, 3);
    
    t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n" , respuesta->mensajes[0]);

    liberar_conexion(socket);
    
    return respuesta->mensajes[0];
    
}

char* enviar_mensaje_obtener_receta(t_modulo* modulo, char* nombre_plato){

    if(nombre_plato == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(obtener_receta);
    int socket;

    char* obtener_receta[2] ={tipo_mensaje, nombre_plato};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, obtener_receta, 2);
    
    t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n" , respuesta->mensajes[0]);

    liberar_conexion(socket);
    
    return respuesta->mensajes[0];
}


List* enviar_mensaje_obtener_receta2(t_modulo* modulo, char* nombre_plato){

    if(nombre_plato == NULL){
        printf("Faltan parametros \n");
        return NULL;
    }

    char* tipo_mensaje = string_itoa(obtener_receta);
    int socket;
    List* lista_pasos_receta = malloc(sizeof(List));

    char* obtener_receta[2] ={tipo_mensaje, nombre_plato};
    socket = send_messages_and_return_socket(modulo->identificacion,modulo->ip, modulo->puerto, obtener_receta, 2);
    
    t_mensajes* respuesta = receive_simple_messages(socket);

    printf("%s \n" , respuesta->mensajes[0]);


    char** respuesta_pasos = string_split(respuesta->mensajes[1], ",");


    for(int i = 0; respuesta_pasos[i]!=NULL; i=i+2){

        t_paso* paso = NULL;

        paso->nombre_paso = respuesta_pasos[i];
        paso->ciclo_cpu = atoi(respuesta_pasos[i+1]);

        pushbacklist(lista_pasos_receta,paso);

    }


    liberar_conexion(socket);
    
    return lista_pasos_receta;
}

char ** separar_por_comillas(char** string_separado_por_espacios){
    List lista_separado_por_comillas;
    initlist(&lista_separado_por_comillas);

    for (int i = 0; string_separado_por_espacios[i] != NULL; i++){

        if (string_starts_with(string_separado_por_espacios[i], "\"")){
            if (string_ends_with(string_separado_por_espacios[i], "\"")){
                char* string_sin_comillas = string_substring(string_separado_por_espacios[i], 1, strlen(string_separado_por_espacios[i]) - 2);
                pushbacklist(&lista_separado_por_comillas, string_sin_comillas);
            } else {
                char* string_concatenado = string_new();
                string_append(&string_concatenado, string_separado_por_espacios[i]);
                i++;
                int finalize_correctamente = 0;
                while(string_separado_por_espacios[i] != NULL){
                    string_append(&string_concatenado, " ");
                    string_append(&string_concatenado, string_separado_por_espacios[i]);
                    if (string_ends_with(string_separado_por_espacios[i], "\"")){
                        finalize_correctamente = 1;
                        break;
                    }
                    i++;
                }
                if (finalize_correctamente == 1){
                    char* string_sin_comillas = string_substring(string_concatenado, 1, strlen(string_concatenado) - 2);
                    pushbacklist(&lista_separado_por_comillas, string_sin_comillas);
                } else {
                    return NULL;
                }
            }
        } else {
            pushbacklist(&lista_separado_por_comillas, string_separado_por_espacios[i]);
        }

    }

    char ** separado_por_comillas = list_a_char(lista_separado_por_comillas);

    return separado_por_comillas;

}

char ** list_a_char(List lista)
{
    int size = sizelist(lista);
    int i = 0;
    char ** resultado = malloc(sizeof(char*) * size);

    for(IteratorList iterator_a = beginlist(lista); iterator_a != NULL; iterator_a = nextlist(iterator_a))
    {
        resultado[i] = iterator_a->data;
        i++;
    }

    return resultado;
}

List* separar_por_comillas_lista(char** string_separado_por_espacios){
    
    List* lista_separado_por_comillas = malloc(sizeof(List));
    initlist(lista_separado_por_comillas);

    for (int i = 0; string_separado_por_espacios[i] != NULL; i++){

        if (string_starts_with(string_separado_por_espacios[i], "\"")){
            if (string_ends_with(string_separado_por_espacios[i], "\"")){
                char* string_sin_comillas = string_substring(string_separado_por_espacios[i], 1, strlen(string_separado_por_espacios[i]) - 2);
                pushbacklist(lista_separado_por_comillas, string_sin_comillas);
            } else {
                char* string_concatenado = string_new();
                string_append(&string_concatenado, string_separado_por_espacios[i]);
                i++;
                int finalize_correctamente = 0;
                while(string_separado_por_espacios[i] != NULL){
                    string_append(&string_concatenado, " ");
                    string_append(&string_concatenado, string_separado_por_espacios[i]);
                    if (string_ends_with(string_separado_por_espacios[i], "\"")){
                        finalize_correctamente = 1;
                        break;
                    }
                    i++;
                }
                if (finalize_correctamente == 1){
                    char* string_sin_comillas = string_substring(string_concatenado, 1, strlen(string_concatenado) - 2);
                    pushbacklist(lista_separado_por_comillas, string_sin_comillas);
                } else {
                    return NULL;
                }
            }
        } else {
            pushbacklist(lista_separado_por_comillas, string_separado_por_espacios[i]);
        }

    }

    return lista_separado_por_comillas;

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

