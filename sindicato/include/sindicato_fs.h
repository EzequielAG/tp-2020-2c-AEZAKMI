#ifndef SINDICATO_FS_H
#define SINDICATO_FS_H

#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <commons/bitarray.h>
#include "sindicato_config.h"

#define BIT_SIZE(x,y) ((x - 1) / y + 1);

// VARIABLES Y ESTRUCTURAS
typedef struct {
    char* block_size;
    char* blocks;
    char* magic_number;
} t_sindicato_metadata;

t_sindicato_metadata* sindicato_metadata;

pthread_mutex_t mutex_bitmap;

// FUNCIONES
void get_or_create_fs();
void crear_metadata_default();
int leer_metadata_afip();
void crear_bitmap();
int get_or_create_folder();
int existe_archivo(char* archivo_path);
int necesita_recrearse(char * block_size, char * blocks, char * magic_number);
void crear_files();
int existe_restaurante(char* restaurante);
int existe_pedido(char* restaurante, char* nro_pedido);
int existe_receta(char* receta);

#endif
