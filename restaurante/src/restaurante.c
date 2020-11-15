#include "restaurante.h"

int main(void){

    sem_id = malloc(sizeof(sem_t));
    sem_init(sem_id, 0, 1);

    //INICIALIZACION CON VARIABLES GLOBALES
    restaurante_init(&restaurante_config, &logger);
   
    modulo_app.ip = restaurante_config->ip_app;
    modulo_app.puerto =restaurante_config->puerto_app;
    modulo_app.identificacion = "app";

    modulo_sindicato.ip = restaurante_config->ip_sindicato;
    modulo_sindicato.puerto = restaurante_config->puerto_sindicato;
    modulo_sindicato.identificacion = "sindicato";
    handshake_init(modulo_app,modulo_sindicato);

    data_restaurante();
   
   inicializar_colas();

//  iniciar_servidor("127.0.0.1", "5002", handle_client);

    

    return 0;
}

// Comienzo handles
void handle_client(t_result* result){

    for(int i = 0; i < *result->mensajes->size; i++){
        printf("%s ", result->mensajes->mensajes[i]);
    }
    printf("\n");

    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);

        if(tipo_mensaje == 16){
            send_message_socket(result->socket, "OK");

        }
        
        if (tipo_mensaje == consultar_platos){
            // HACER SI HAY PLATOS
            List* platos = enviar_mensaje_consultar_platos(&modulo_sindicato, restaurante_config->nombre_restaurante);
            int cant_platos = sizelist(*platos);

            if(cant_platos == 0)
            {
                send_messages_socket(result->socket,list_a_char(*platos), cant_platos);
            }

            } else if (tipo_mensaje == crear_pedido) {

                handle_crear_pedido(result->socket);

            } else if (tipo_mensaje == anadir_plato) {

                handle_anadir_plato(result);

            } else if (tipo_mensaje == confirmar_pedido) {
            
                handle_confirmar_pedido(result);
                
            } else if (tipo_mensaje == consultar_pedido) {
                // TODO : FALTA LOGICA CONSULTAR_PEDIDO
            }
    }
}

void handle_crear_pedido(int socket){

    int id = asignar_pedido_id();
       
    char* respuesta = enviar_mensaje_guardar_pedido(&modulo_sindicato, restaurante_config->nombre_restaurante,string_itoa(id));

    if(!strcmp(respuesta,"OK")){
        send_message_socket(socket,string_itoa(id));
    }else{
        send_message_socket(socket,"FAIL");
    }
}

void handle_anadir_plato(t_result* result){
 
    char* respuesta = enviar_mensaje_guardar_plato(&modulo_sindicato, restaurante_config->nombre_restaurante,result->mensajes->mensajes[2] ,result->mensajes->mensajes[1] , "1");

    send_message_socket(result->socket,respuesta);

};


void handle_confirmar_pedido(t_result* result){


}

void handle_obtener_restaurante(r_obtener_restaurante* respuesta){
   
    afinidades = *respuesta->afinidades;
    strcpy(pos_x,respuesta->pos_x);
    strcpy(pos_y,respuesta->pos_y);
    cantidad_hornos = atoi(respuesta->cantidad_hornos);
    cantidad_pedidos = atoi(respuesta->cantidad_pedidos);
    cantidad_cocineros = atoi(respuesta->cantidad_cocineros);
    recetas = respuesta->recetas_precio; // REVISAR ESTO

}


void inicializacion_default(){
   r_obtener_restaurante* resto_default = malloc(sizeof(r_obtener_restaurante));


   List* afinidades_default = malloc(sizeof(List));
   initlist(afinidades_default);
   pushbacklist(afinidades_default,"Milanesa");
   pushbacklist(afinidades_default,"Pizza");


   resto_default->afinidades = afinidades_default;
   resto_default->pos_x = "2";
   resto_default->pos_y = "3";
   resto_default->cantidad_cocineros = "3";
   resto_default->cantidad_pedidos = "2";
   resto_default->cantidad_hornos = "2";
   resto_default->recetas_precio = NULL;

    handle_obtener_restaurante(resto_default);

}




// Finalizo handles

// Comienzo visuales
void ver_info_pedido(List* lista_pedidos){
 
     for(IteratorList iter_pedido = beginlist(*lista_pedidos); iter_pedido != NULL; iter_pedido = nextlist(iter_pedido))
     {
        t_pedido* pedido = iter_pedido->data;
        printf("Estoy en el pedido : %d \n", pedido->id_pedido);
        printf("Y tiene los siguientes platos: \n");
        for(IteratorList iter_platos = beginlist((pedido->platos)); iter_platos != NULL; iter_platos = nextlist(iter_platos)){
            t_plato* plato = iter_platos->data;
            printf(" - Nombre plato: %s \n", plato->nombre);
            printf("Pasos sin ejecutar: \n");
              for(IteratorList iter_pasos = beginlist(*(plato->pasos)); iter_pasos != NULL; iter_pasos = nextlist(iter_pasos)){
                 t_paso* paso_plato = iter_pasos->data;
                
                 if(paso_plato->se_ejecuto){
                 printf(" - Paso: %s \n", paso_plato->nombre_paso);
                 printf(" - Ciclo cpu: %d \n", paso_plato->ciclo_cpu);
                 printf(" - Es io : %d \n", es_paso_io(paso_plato));

                 printf("--------\n");
                }
              
              }

            
        }
        printf("\n");
        printf("\n");
        
    }
};


void data_restaurante(){

    for(IteratorList iterator_afinidades = beginlist(afinidades); iterator_afinidades != NULL; iterator_afinidades = nextlist(iterator_afinidades))
    {
        printf("<< RESTAURANTE >> Iniciado con afinidades = %s \n", (char*)iterator_afinidades->data);
    }

    printf("<< RESTAURANTE >> Iniciado con posiciones x = %s ; y = %s\n", pos_x,pos_y);
    
    for(int j = 0;j< 0;j++)  {

        printf("<< RESTAURANTE >> Iniciado con recetas = %s %s\n", recetas[j]->precio, recetas[j]->receta);
    }

    printf("<< RESTAURANTE >> Iniciado con cantidad de hornos = %d\n", cantidad_hornos);
    printf("<< RESTAURANTE >> Iniciado con cantidad de cocineros = %d\n", cantidad_cocineros);
    printf("<< RESTAURANTE >> Iniciado con cantidad de pedidos = %i\n", cantidad_pedidos);

}



// Finalizo visuales



//COMIENZO HANDSHAKES

int handshake(t_modulo* modulo){

    char* mensajes[2] = {string_itoa(handshake_restaurante), restaurante_config->nombre_restaurante};

    socket_sindicato = send_messages_and_return_socket(restaurante_config->nombre_restaurante, modulo->ip, modulo->puerto, mensajes, 2);

    if (socket_sindicato == -1){
        return -1;
    }

    char * mensaje = receive_simple_message(socket_sindicato);

    if (mensaje == NULL){
        return -1;
    }

    printf("El handshake con el modulo %s fue correcto\n", modulo->identificacion);

    escuchar_mensajes_socket_desacoplado(socket_sindicato);

    return 0;
}

void escuchar_mensajes_socket_desacoplado(int socket){
    
    pthread_t thread;
    t_parameter* parametro = malloc(sizeof(t_parameter));

	parametro->socket = socket;
	parametro->f = handle_client;

	pthread_create(&thread,NULL,(void*)escuchar_mensajes_socket, parametro);
	pthread_detach(thread);

}

void escuchar_mensajes_socket(t_parameter* parametro){
    escuchar_socket(&parametro->socket, parametro->f);
}


// DATOS RESTAURANTE

void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger){
    *restaurante_config = restaurante_config_loader("./cfg/restaurante.config");
    *logger = init_logger((*restaurante_config)->ruta_log, "restaurante", LOG_LEVEL_INFO);
    pos_x = malloc(sizeof(char*));
    pos_y = malloc(sizeof(char*));
    recetas = malloc(sizeof(receta_precio**));
    cantidad_pedidos = 0;
    cantidad_platos = 0;
    initlist(&lista_pedidos);
    initlist(&colas_ready);
    initlist(&colas_exit);
    initlist(&colas_block);
   

}

void restaurante_finally(t_restaurante_config* restaurante_config, t_log* logger) {
    restaurante_destroy(restaurante_config);
    log_destroy(logger);
}


t_restaurante_config* restaurante_config_loader(char* path_config_file) {
    t_config* config = config_create(path_config_file);
    t_restaurante_config* restaurante_config = malloc(sizeof(t_restaurante_config));

    restaurante_config_parser(config, restaurante_config);
    config_destroy(config);

    return restaurante_config;
}

void restaurante_config_parser(t_config* config, t_restaurante_config* restaurante_config) {
    restaurante_config->puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
    restaurante_config->ip_sindicato = strdup(config_get_string_value(config, "IP_SINDICATO"));
    restaurante_config->puerto_sindicato = strdup(config_get_string_value(config, "PUERTO_SINDICATO"));
    restaurante_config->ip_app = strdup(config_get_string_value(config, "IP_APP"));
    restaurante_config->puerto_app = strdup(config_get_string_value(config, "PUERTO_APP"));
    restaurante_config->quantum = config_get_int_value(config, "QUANTUM");
    restaurante_config->ruta_log = strdup(config_get_string_value(config, "ARCHIVO_LOG"));
    restaurante_config->algoritmo_planificador = strdup(config_get_string_value(config, "ALGORITMO_PLANIFICACION"));
    restaurante_config->nombre_restaurante = strdup(config_get_string_value(config, "NOMBRE_RESTAURANTE"));
}

void restaurante_destroy(t_restaurante_config* restaurante_config) {
    free(restaurante_config->puerto_escucha);
    free(restaurante_config->ip_sindicato);
    free(restaurante_config->puerto_sindicato);
    free(restaurante_config->ip_app);
    free(restaurante_config->puerto_app);
    free(restaurante_config->ruta_log);
    free(restaurante_config->algoritmo_planificador);
    free(restaurante_config->nombre_restaurante);
    free(restaurante_config);
}

void handshake_init(t_modulo modulo1, t_modulo modulo2){
    
    int handshake_app_r = handshake(&modulo1);

    if (handshake_app_r == -1){
        printf("No se pudo realizar la conexion inicial con el modulo app\n");
    }

    int handshake_sindicato_r = handshake(&modulo2);

    if (handshake_sindicato_r == -1){
        printf("No se pudo realizar la conexion inicial con el modulo sindicato\n");
        inicializacion_default();
    }
    else{
        handle_obtener_restaurante(enviar_mensaje_obtener_restaurante(&modulo2, restaurante_config->nombre_restaurante));
    }

}



// FUNCIONES AUXILIARES 

int len_array(char** arrayInput)
{
    int i = 0;
    int cont = 0;
 
    for(i=0 ; arrayInput[i] != NULL ; i++)
    {
       cont = cont + 1; 
    }
        
    return cont;
}





int asignar_pedido_id(){

    sem_wait(sem_id);
    cantidad_pedidos = cantidad_pedidos + 1;
    sem_post(sem_id);
        
    return cantidad_pedidos;
}


char* conveRecetasString(receta_precio** recetas)
{
    char* a = string_new();
			
    for(int i=0 ; i < sizeof(recetas); i++)
	{
        string_append(&a,recetas[i]->receta);
        string_append(&a," | ");
        string_append(&a,recetas[i]->precio);
        string_append(&a," , ");
	}

    return a;
}
int es_paso_io(t_paso* paso){

    if( !strcmp((paso->nombre_paso),"HORNEAR") || !strcmp((paso->nombre_paso),"Hornear")){
        return 1;
    }

   return 0;
}