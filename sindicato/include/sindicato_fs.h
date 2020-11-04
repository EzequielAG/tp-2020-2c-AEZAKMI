#ifndef SINDICATO_FS_H
#define SINDICATO_FS_H

#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include "sindicato_config.h"

#define BIT_SIZE(x,y) ((x - 1) / y + 1);

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* block_size;
    char* blocks;
    char* magic_number;
} t_sindicato_metadata;

typedef struct {
	int x;
	int y;
} t_posicion;

typedef struct {
    int cantidad_cocineros;
    t_posicion* posicion;
    t_list* afinidad_cocineros;
    t_list* platos;
    t_list* precio_platos;
    int cantidad_hornos;
} t_info_file;

typedef struct {
    char* estado_pedido;
    t_list* lista_platos;
    t_list* cantidad_platos;
    t_list* cantidad_lista;
    int precio_total;
} t_pedido_file;

typedef struct {
    t_list* pasos;
    t_list* tiempo_paso;
} t_receta_file;

t_sindicato_metadata* sindicato_metadata;

pthread_mutex_t mutex_bitmap;

// FUNCIONES
char* get_path_restaurante(char* restaurante);
char* get_path_info_file(char* restaurante);
char* get_path_pedido_file(char* restaurante, char* id_pedido);
char* get_path_receta_file(char* nombre_receta);
void get_or_create_fs();
void crear_metadata_default();
int leer_metadata_afip();
void crear_bitmap();
int get_or_create_folder();
int existe_archivo(char* archivo_path);
int necesita_recrearse(char * block_size, char * blocks, char * magic_number);
void crear_files();
bool existe_restaurante(char* restaurante);
int existe_pedido(char* restaurante, char* nro_pedido);
int existe_receta(char* receta);

#endif
