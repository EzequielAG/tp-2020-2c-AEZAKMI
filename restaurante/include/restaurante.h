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

// FUNCIONES
void restaurante_init(t_restaurante_config** restaurante_config, t_log** logger);
void restaurante_finally(t_restaurante_config* restaurante_config, t_log* logger);
t_restaurante_config* restaurante_config_loader(char* path_config_file);
void restaurante_config_parser(t_config* config, t_restaurante_config* restaurante_config);
void restaurante_destroy(t_restaurante_config* restaurante_config);
int obtener_id_pedido();
void handle_crear_pedido(int socket);

#endif