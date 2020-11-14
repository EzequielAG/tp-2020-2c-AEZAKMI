#ifndef APP_H
#define APP_H
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "shared_utils.h"
#include "server.h"
#include "list.h"
#include "api.h"
#include "tests.h"
#include "app_messages.h"

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
<<<<<<< Updated upstream
void handle_consultar_restaurantes(int socket);
int relacionar(char* restaurante, char* cliente);
void handle_seleccionar_restaurante(int socket, char* restaurante, char* cliente);
int obtener_id_pedido();
void handle_crear_pedido(int socket);
t_cliente* buscar_cliente_lista(char* id_cliente);
void handle_handshake_cliente(int socket, char* id_cliente);
t_restaurante* buscar_restaurante_lista(char* nombre_restaurante);
void handle_handshake_restaurante(int socket, char* nombre_restaurante);
t_restaurante* nuevo_restaurante(int socket, char* nombre_restaurante);
char* obtener_restaurantes();
void handle_client(t_result* result);
=======
>>>>>>> Stashed changes

#endif