#ifndef CLIENTE_H
#define CLIENTE_H
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* ip_comanda;
    char* puerto_comanda;
    char* ip_restaurante;
    char* puerto_restaurante;
    char* ip_sindicato;
    char* puerto_sindicato;
    char* ip_app;
    char* puerto_app;
    char* ruta_log;
    int posicion_x;
    int posicion_y;
} t_cliente_config;

t_cliente_config* cliente_config;
t_log* logger;

// FUNCIONES
void cliente_init(t_cliente_config** cliente_config, t_log** logger);
void cliente_finally(t_cliente_config* cliente_config, t_log* logger);
t_cliente_config* cliente_config_loader(char* path_config_file);
void cliente_config_parser(t_config* config, t_cliente_config* cliente_config);
void cliente_destroy(t_cliente_config* cliente_config);

#endif