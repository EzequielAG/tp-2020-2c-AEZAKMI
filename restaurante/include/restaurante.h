#ifndef RESTAURANTE_H
#define RESTAURANTE_H
#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <semaphore.h>
#include "server.h"
#include "tests.h"
#include "planificacion.h"

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


// FUNCIONES
void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger);
void restaurante_finally(t_restaurante_config* restaurante_config, t_log* logger);
t_restaurante_config* restaurante_config_loader(char* path_config_file);
void restaurante_config_parser(t_config* config, t_restaurante_config* restaurante_config);
void restaurante_destroy(t_restaurante_config* restaurante_config);
int asignar_pedido_id();
int handshake(t_modulo* modulo);
int handshake_app(t_modulo modulo_app);
void escuchar_mensajes_socket_desacoplado(int socket);
void escuchar_mensajes_socket(t_parameter* parametro);
void handle_client(t_result* result);
void inicializacion_default();
void handle_obtener_restaurante(r_obtener_restaurante* resultado);
void inicializar(List*,char*,char*,receta_precio**,char*,int,char*);
int len_array(char** arrayInput);
void consultar_platos_f();
void handle_crear_pedido(int socket);
void handle_anadir_plato(t_result* result);
void handle_confirmar_pedido(t_result* result);
void data_restaurante();
void casos_uso();
void ver_info_pedido(List* lista_pedidos);
int es_paso_io(t_paso* paso);
sem_t* sem_id;

#endif