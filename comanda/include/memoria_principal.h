#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include "server.h"
#include "tests.h"
#include "api.h"
#include "list.h"
#include "comanda.h"



typedef struct segmento{

    //int numeroSegmento;
    char* idPedido;
    List* punteroTablaPaginas;
    
} l_segmento;

typedef struct frame{

    uint32_t cantidadPlato;
    uint32_t cantidadLista;
    char plato[24];

} l_frame;

typedef struct pagina{

    int numeroPagina;
    int bitPresencia;
    l_frame *frame;

} l_pagina;

typedef struct proceso{

    char* nombreResto;
    List* punteroTablaSegmentos;

} l_proceso;

void* puntero_memoria_principal;
l_segmento *crearSegmento();
void crear_pagina(l_segmento*, void*);