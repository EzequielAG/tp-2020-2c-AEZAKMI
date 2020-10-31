#include "restaurante.h"

int main(void){

    //ESTE VA A ACTUAR DE SERVER
    restaurante_init(&restaurante_config, &logger);
    log_info(logger, "Soy el MODULO RESTAURANTE! %s", mi_funcion_compartida());
    printf("Imprimiendo el path %s", restaurante_config->ruta_log);
    
    //VARIABLES GLOBALES
    afinidades = malloc(sizeof(char) * 2);
    pos_x = malloc(sizeof(char));
    pos_y = malloc(sizeof(char));
    recetas = malloc(sizeof(receta_precio) * 2);
    cantidad_hornos = malloc(sizeof(char));
    cantidad_pedidos = malloc(sizeof(char));

    //TODO: Hacer que reciba ip y puerto de config
    //iniciar_servidor("127.0.0.1", "5002", handle_client);

    //0. Inicializo modulos a los que me voy a tener que conectar
    t_modulo modulo_app = {restaurante_config->ip_app, restaurante_config->puerto_app, "app"};
    t_modulo modulo_sindicato = {restaurante_config->ip_sindicato, restaurante_config->puerto_sindicato, "sindicato"};

    //1.1 Handshake con el modulo app
    int handshake_app_r = handshake_app(modulo_app);

    //1.2 Handshake con el modulo sindicato
    //Obtencion de metadata del restaurante
    //Envia obtener_restaurante(nombre) al sindicato, el cual le devolvera la informacion de este restaurante

    int handshake_sindicato_r = handshake(&modulo_sindicato);

    if (handshake_sindicato_r == -1){
        printf("No se pudo realizar la conexion inicial con el modulo app\n");
        inicializacion_default();
        return -1;
    }
    else{
        char* mensajes[2] = {"obtener_restaurante", restaurante_config->nombre_restaurante};
        send_messages_socket(socket_sindicato, mensajes, 2);
    }

    printf("<< RESTAURANTE >> Iniciado con afinidades = %s\n", afinidades);
    printf("<< RESTAURANTE >> Iniciado con posiciones x = %s ; y = %s\n", pos_x,pos_y);
    printf("<< RESTAURANTE >> Iniciado con recetas = %s\n", recetas);
    printf("<< RESTAURANTE >> Iniciado con cantidad de hornos = %s\n", cantidad_hornos);
    printf("<< RESTAURANTE >> Iniciado con cantidad de pedidos = %s\n", cantidad_pedidos);

    //2. Creacion/inicializacion de colas de planificacion

    restaurante_finally(restaurante_config, logger);
    return 0;
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

int obtener_id_pedido(){
    //TODO: Obtener un id de pedido unico para la instancia del restaurante
    int id_pedido = 435;
    return id_pedido;
}

void handle_crear_pedido(int socket){

    int id_pedido = obtener_id_pedido();
    char* respuesta[1];

    respuesta[0] = string_itoa(id_pedido);

    t_modulo modulo_sindicato;
    modulo_sindicato.ip = restaurante_config->ip_sindicato;
    modulo_sindicato.puerto = restaurante_config->puerto_sindicato;
    modulo_sindicato.nombre = "Comanda";

    //TODO: De donde saco el restaurante?
    enviar_mensaje_guardar_pedido(&modulo_sindicato, "Restaurante1", respuesta[0]);

    send_messages_socket(socket, respuesta, 1);
    liberar_conexion(socket);
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

void handle_client(t_result* result){

    for(int i = 0; i < *result->mensajes->size; i++){
        printf("%s", result->mensajes->mensajes[i]);
    }
    printf("\n");

    if (result->operacion == MENSAJES){
        int tipo_mensaje = atoi(result->mensajes->mensajes[0]);
        if (tipo_mensaje == consultar_platos){
            // TODO : FALTA LOGICA CONSULTAR_PLATOS
        } else if (tipo_mensaje == crear_pedido) {
            // TODO : FALTA LOGICA CREAR_PEDIDO
        } else if (tipo_mensaje == anadir_plato) {
            // TODO : FALTA LOGICA ANADIR_PLATO
        } else if (tipo_mensaje == confirmar_pedido) {
            // TODO : FALTA LOGICA CONFIRMAR_PEDIDO
        } else if (tipo_mensaje == consultar_pedido) {
            // TODO : FALTA LOGICA CONSULTAR_PEDIDO
        } else if (tipo_mensaje == obtener_restaurante) {

            r_obtener_restaurante* este_restaurante = malloc(sizeof(r_obtener_restaurante));
            este_restaurante->afinidades = obtener_array_mensajes(result->mensajes->mensajes[0]);;
            este_restaurante->pos_x = result->mensajes->mensajes[1];
            este_restaurante->pos_y = result->mensajes->mensajes[2];
            este_restaurante->recetas = obtener_receta_precios(result->mensajes->mensajes[3]) ;
            este_restaurante->cantidad_hornos = result->mensajes->mensajes[4];
            este_restaurante->cantidad_pedidos = result->mensajes->mensajes[5];
            
            handle_obtener_restaurante(este_restaurante);
        }
    }
}

void handle_obtener_restaurante(r_obtener_restaurante* respuesta){
//Se ejecutará el mensaje Obtener Restaurante al módulo Sindicato obteniendo todos los datos del mismo, siendo algunos de ellos:
// Cantidad de cocineros y sus afinidades, posición del restaurante en el mapa, recetas disponibles con sus precios y 
//la cantidad de hornos. A su vez, deberá retornar la cantidad de pedidos que ya disponga para no pisar los ID con los
// nuevos generados.
inicializar(respuesta->afinidades,respuesta->pos_x,respuesta->pos_y,respuesta->recetas,respuesta->cantidad_hornos,respuesta->cantidad_pedidos);
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

receta_precio* receta1 = malloc(sizeof(receta_precio));
receta1->receta = "Milanesas";
receta1->precio = "400";

receta_precio* receta2 = malloc(sizeof(receta_precio));
receta2->receta = "Empanadas";
receta2->precio = "350";

receta_precio* receta3 = malloc(sizeof(receta_precio));
receta3->receta = "Ensalada";
receta3->precio = "300";

receta_precio** recetas = malloc(sizeof(receta_precio)*3);
recetas[0] = receta1;
recetas[1] = receta2;
recetas[2] = receta3;
char* af[2] = {"Milanesas","Empanadas"};

inicializar(af,"4","5",recetas,"2","6");

}

void inicializar(char** afinidades_f,char* pos_x_f,char* pos_y_f,receta_precio** recetas_f,char* cantidad_hornos_f,char* cantidad_pedidos_f){

afinidades = afinidades_f;
pos_x = pos_x_f;
pos_y = pos_y_f;
recetas = recetas_f;
cantidad_hornos = cantidad_hornos_f;
cantidad_pedidos = cantidad_pedidos_f;

}