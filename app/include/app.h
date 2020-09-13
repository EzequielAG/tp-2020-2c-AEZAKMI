#ifndef APP_H
#define APP_H
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "server.h"
#include "api.h"
#include "tests.h"

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* ip_comanda;
    char* puerto_comanda;
    char* puerto_escucha;
    int grado_multiprocesamiento;
    char* algoritmo_planificacion;
    double alpha;
    int estimacion_inicial;
    int repartidores;
    int frecuencia_descanso;
    int tiempo_descanso;
    char* ruta_log;
    int platos_default;
    int posicion_rest_default_x;
    int posicion_rest_default_y;
} t_app_config;

t_app_config* app_config;
t_log* logger;

// FUNCIONES
void app_init(t_app_config** app_config, t_log** logger);
void app_finally(t_app_config* app_config, t_log* logger);
t_app_config* app_config_loader(char* path_config_file);
void app_config_parser(t_config* config, t_app_config* app_config);
void app_destroy(t_app_config* app_config);
void handle_consultar_restaurantes(int socket);
int relacionar(char* restaurante, char* cliente);
void handle_seleccionar_restaurante(int socket, char* restaurante, char* cliente);

#endif