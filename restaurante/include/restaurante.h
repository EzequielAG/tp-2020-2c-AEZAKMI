#ifndef RESTAURANTE_H
#define RESTAURANTE_H
#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"
#include "api.h"

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* puerto_escucha;
    char* ip_sindicato;
    char* puerto_sindicato;
    char* ip_app;
    char* puerto_app;
    int quantum;
    char* ruta_log;
    char* algoritmo_planificador;
    char* nombre_restaurante;
} t_restaurante_config;

t_restaurante_config* restaurante_config;
t_log* logger;

// typedef struct{
//     char* receta;
//     char* precio;
// } receta_precio;

//VARIABLES GLOBALES
char** afinidades;
char* pos_x;
char* pos_y;
receta_precio** recetas;
char* cantidad_hornos;
char* cantidad_pedidos;
int socket_sindicato;
    
// FUNCIONES
void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger);
void restaurante_finally(t_restaurante_config* restaurante_config, t_log* logger);
t_restaurante_config* restaurante_config_loader(char* path_config_file);
void restaurante_config_parser(t_config* config, t_restaurante_config* restaurante_config);
void restaurante_destroy(t_restaurante_config* restaurante_config);
int obtener_id_pedido();
void handle_crear_pedido(int socket);
int handshake(t_modulo* modulo);
int handshake_app(t_modulo modulo_app);
void escuchar_mensajes_socket_desacoplado(int socket);
void escuchar_mensajes_socket(t_parameter* parametro);
void handle_client(t_result* result);
void inicializacion_default();
void handle_obtener_restaurante(r_obtener_restaurante* resultado);
void inicializar(char**,char*,char*,receta_precio**,char*,char*);

#endif