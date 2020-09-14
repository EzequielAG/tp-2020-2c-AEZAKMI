#ifndef SINDICATO_H
#define SINDICATO_H
#include <stdio.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"
#include "api.h"

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* puerto_escucha;
    char* punto_montaje;
    char* ruta_log;
} t_sindicato_config;

t_sindicato_config* sindicato_config;
t_log* logger;

// FUNCIONES
void sindicato_init(t_sindicato_config** sindicato_config, t_log** logger);
void sindicato_finally(t_sindicato_config* sindicato_config, t_log* logger);
t_sindicato_config* sindicato_config_loader(char* path_config_file);
void sindicato_config_parser(t_config* config, t_sindicato_config* sindicato_config);
void sindicato_destroy(t_sindicato_config* sindicato_config);
int guardar_pedido_en_afip(char* restaurante, char* id_pedido);
void handle_guardar_pedidos(int socket, char* restaurante, char* id_pedido);
void handle_client(t_result* result);

#endif