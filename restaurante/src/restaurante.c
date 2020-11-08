#include "restaurante.h"

int main(void){

    sem_id = malloc(sizeof(sem_t));
    sem_init(sem_id, 0, 1);

    //ESTE VA A ACTUAR DE SERVER
    restaurante_init(&restaurante_config, &logger);
    log_info(logger, "Soy el MODULO RESTAURANTE! %s", mi_funcion_compartida());
    
    //MALLOC VARIABLES GLOBALES
    afinidades = malloc(sizeof(List));
    pos_x = malloc(sizeof(char*));
    pos_y = malloc(sizeof(char*));
    recetas = malloc(sizeof(receta_precio**));
    cantidad_hornos = malloc(sizeof(char*));
    cantidad_pedidos = 0;
    cantidad_platos = 0;
    cantidad_cocineros = malloc(sizeof(char*));
    initlist(&l_pedidos);

    //TODO: Hacer que reciba ip y puerto de config
    //iniciar_servidor("127.0.0.1", "5002", handle_client);

    //0. Inicializo modulos a los que me voy a tener que conectar
    //t_modulo modulo_app = {restaurante_config->ip_app, restaurante_config->puerto_app, "app"};
    modulo_sindicato.ip = restaurante_config->ip_sindicato;
    modulo_sindicato.puerto = restaurante_config->puerto_sindicato;
    modulo_sindicato.nombre = "sindicato";

    //1.1 Handshake con el modulo app
    //int handshake_app_r = handshake_app(modulo_app);

    //1.2 Handshake con el modulo sindicato
    //Obtencion de metadata del restaurante
    //Envia obtener_restaurante(nombre) al sindicato, el cual le devolvera la informacion de este restaurante

    int handshake_sindicato_r = handshake(&modulo_sindicato);

    if (handshake_sindicato_r == -1){
        printf("No se pudo realizar la conexion inicial con el modulo sindicato\n");
        inicializacion_default();
    }
    else{
        handle_obtener_restaurante(enviar_mensaje_obtener_restaurante(&modulo_sindicato, restaurante_config->nombre_restaurante));
    }

    //PRINTF AFINIDIDADES Y ESO

    for(IteratorList iterator_afinidades = beginlist(*afinidades); iterator_afinidades != NULL; iterator_afinidades = nextlist(iterator_afinidades))
    {
        printf("<< RESTAURANTE >> Iniciado con afinidades = %s \n", (char*)iterator_afinidades->data);
    }

    printf("<< RESTAURANTE >> Iniciado con posiciones x = %s ; y = %s\n", pos_x,pos_y);
    
    for(int j = 0;j< 0;j++)  {

        printf("<< RESTAURANTE >> Iniciado con recetas = %s %s\n", recetas[j]->precio, recetas[j]->receta);
    }

    printf("<< RESTAURANTE >> Iniciado con cantidad de hornos = %s\n", cantidad_hornos);
    printf("<< RESTAURANTE >> Iniciado con cantidad de pedidos = %i\n", cantidad_pedidos);

    //2. Creacion/inicializacion de colas de planificacion
    inicializar_colas();


    restaurante_finally(restaurante_config, logger);
    return 0;
}

void handle_client(t_result* result){

    for(int i = 0; i < *result->mensajes->size; i++){
        printf("%s", result->mensajes->mensajes[i]);
    }
    printf("\n");

    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        
        if (tipo_mensaje == consultar_platos){
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


int asignar_pedido_id(){

    sem_wait(sem_id);
    cantidad_pedidos = cantidad_pedidos + 1;
    sem_post(sem_id);
        
    return cantidad_pedidos;
}

void handle_crear_pedido(int socket){

    int id = asignar_pedido_id();
    t_pedido* pedido = malloc(sizeof(t_pedido));
    pedido->id = id;
    initlist(&pedido->platos);
    
    //lo pusheo en la cola de pedidos con el id respectivo
    if(pushbacklist(&l_pedidos,pedido) == 1)
    {
        //lo mando a guardar al sindicato
        char* respuesta = enviar_mensaje_guardar_pedido(&modulo_sindicato, restaurante_config->nombre_restaurante,string_itoa(id));
        if(!strcmp(respuesta,"OK")){
            send_message_socket(socket,string_itoa(id));
        }else{
            send_message_socket(socket,"FAIL");
        }
    }
}

void handle_anadir_plato(t_result* result){

     for(IteratorList iter = beginlist(l_pedidos); iter != NULL; iter = nextlist(iter)) {
        t_pedido* pedido = iter->data;

        if((pedido->id) == atoi(result->mensajes->mensajes[2])){
            pushbacklist(&(pedido->platos), result->mensajes->mensajes[1]);
            // REVISAR CANTIDAD 
            char* respuesta = enviar_mensaje_guardar_plato(&modulo_sindicato, restaurante_config->nombre_restaurante,result->mensajes->mensajes[2] ,result->mensajes->mensajes[1] , "1");

            send_message_socket(result->socket,respuesta);

            return;

        }

        
    }

    send_message_socket(result->socket,"FAIL");

};


void handle_confirmar_pedido(t_result* result){


    // OBTENER PEDIDO DEL SINDICATO
    
    r_obtener_pedido2* pedido = enviar_mensaje_obtener_pedido2(&(modulo_sindicato), result->mensajes->mensajes[1], restaurante_config->nombre_restaurante);

    t_pedido* pedido_restaurante = malloc(sizeof(t_pedido));
    pedido_restaurante->id = atoi(result->mensajes->mensajes[1]);
    
    
    if(strcmp(pedido->estado,"LISTO"))
    {
        for(IteratorList iter = beginlist(*(pedido->info_comidas)); iter != NULL; iter = nextlist(iter))
        {
            informacion_comidas* info_comida = iter->data;
            List* receta_comida = enviar_mensaje_obtener_receta2(&modulo_sindicato, info_comida->comida);

            t_plato* plato = malloc(sizeof(t_plato));
            plato->pcb->pid = cantidad_platos + 1;
            cantidad_platos++;
            plato->cantidad_total = atoi(info_comida->cantidad_total);
            plato->cantidad_listo = atoi(info_comida->cantidad_lista);

            strcpy(plato->nombre,info_comida->comida);

            for(IteratorList iter2 = beginlist(*receta_comida); iter2 != NULL; iter2 = nextlist(iter2)){
                t_paso* paso = iter2->data;
                t_pasos_platos* paso_plato = malloc(sizeof(t_pasos_platos));
                paso_plato->nombre = paso->nombre_paso;
                paso_plato->ciclos_cpu = atoi(paso->ciclo_cpu);
                paso_plato->se_ejecuto = 0;
                if(!strcmp(paso->nombre_paso, "HORNEAR"))
                {
                        paso_plato->es_io = 1;
                }  
                else{
                        paso_plato->es_io = 0;
                }
                pushbacklist(&plato->pasos,paso_plato);
                
            }
            pushbacklist(&pedido_restaurante->platos,plato);
            paso_new(plato);
        }
        pushbacklist(&l_pedidos,pedido_restaurante);
    
        // INFORMAR AL MODULO QUE CONFIRMO EL PEDIDO QUE SU PEDIDO FUE CONFIRMADO.
        send_message_socket(result->socket,"OK");
    }

}

int handshake(t_modulo* modulo){

    char* mensajes[2] = {string_itoa(handshake_restaurante), restaurante_config->nombre_restaurante};

    socket_sindicato = send_messages_and_return_socket(modulo->ip, modulo->puerto, mensajes, 2);

    if (socket_sindicato == -1){
        return -1;
    }

    char * mensaje = receive_simple_message(socket_sindicato);

    if (mensaje == NULL){
        return -1;
    }

    printf("El handshake con el modulo %s fue correcto\n", modulo->nombre);

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



void handle_obtener_restaurante(r_obtener_restaurante* respuesta){
//Se ejecutará el mensaje Obtener Restaurante al módulo Sindicato obteniendo todos los datos del mismo, siendo algunos de ellos:
// Cantidad de cocineros y sus afinidades, posición del restaurante en el mapa, recetas disponibles con sus precios y 
//la cantidad de hornos. A su vez, deberá retornar la cantidad de pedidos que ya disponga para no pisar los ID con los
// nuevos generados.
inicializar(respuesta->afinidades,respuesta->pos_x,respuesta->pos_y,respuesta->recetas,respuesta->cantidad_hornos,atoi(respuesta->cantidad_pedidos),respuesta->cantidad_cocineros);
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


void inicializacion_default(){

    receta_precio* receta1 = malloc(sizeof(receta_precio*));
    receta1->receta = "Milanesas";
    receta1->precio = "400";

    receta_precio* receta2 = malloc(sizeof(receta_precio*));
    receta2->receta = "Empanadas";
    receta2->precio = "350";

    receta_precio* receta3 = malloc(sizeof(receta_precio*));
    receta3->receta = "Ensalada";
    receta3->precio = "300";

    receta_precio** recetas = malloc(sizeof(receta_precio**));
    recetas[0] = receta1;
    recetas[1] = receta2;
    recetas[2] = receta3;

    List* afinidades_default = malloc(sizeof(List));
    initlist(afinidades_default);
    pushbacklist(afinidades_default, "Milanesas");
    pushbacklist(afinidades_default, "Empanadas");

    inicializar(afinidades_default,"4","5",recetas,"2",6,"2");

}

void inicializar(List* afinidades_f,char* pos_x_f,char* pos_y_f,receta_precio** recetas_f,char* cantidad_hornos_f,int cantidad_pedidos_f,char* cantidad_cocineros_f){

    for(IteratorList iterator_afinidades = beginlist(*afinidades_f); iterator_afinidades != NULL; iterator_afinidades = nextlist(iterator_afinidades))
    {
        pushbacklist(afinidades, iterator_afinidades->data);
    }

    strcpy(pos_x,pos_x_f);
    strcpy(pos_y,pos_y_f);
    strcpy(cantidad_hornos,cantidad_hornos_f);
    cantidad_pedidos = cantidad_pedidos_f;
    strcpy(cantidad_cocineros,cantidad_cocineros_f);

    recetas = recetas_f;
}

void consultar_platos_f()
{
};


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





int handshake_app(t_modulo modulo_app)
{
    int respuesta = handshake(&modulo_app);

    if (respuesta == -1){
        printf("No se pudo realizar la conexion inicial con el modulo app\n");
        return -1;
    }

    return respuesta;
}

void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger){
    *restaurante_config = restaurante_config_loader("./cfg/restaurante.config");
    *logger = init_logger((*restaurante_config)->ruta_log, "restaurante", LOG_LEVEL_INFO);
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