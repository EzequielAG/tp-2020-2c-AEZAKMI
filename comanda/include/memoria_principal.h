#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <stdbool.h>
#include "shared_utils.h"
#include <commons/bitarray.h>
#include "server.h"
#include "tests.h"
#include "api.h"
#include "list.h"



typedef struct segmento{

    //int numeroSegmento;
    char* idPedido;
    int estadoPedido; // 0 EN PROCESO ; 1 LISTO ; 2 TERMINADO
    List* punteroTablaPaginas;
    
} l_segmento;

typedef struct frame{

    uint32_t cantidadPlato;
    uint32_t cantidadLista;
    char plato[24];

} l_frame;

typedef struct puntero{

    int bitOcupado;
    void *punteroFrame;

} l_punteroFrame;

typedef struct pagina{

    int bitUso;
    int bitPresencia;
    void *frame;

} l_pagina;

typedef struct proceso{

    char* nombreResto;
    List* punteroTablaSegmentos;

} l_proceso;

void *puntero_memoria_principal;
void *puntero_memoria_swap;
int crearSegmento(l_proceso*, char* idPedido);
void crear_pagina(l_segmento*, int cantidad, char plato[24]);
l_proceso *find_resto_lista(char*);
l_proceso *crearProceso(char *);
void imprimirMemoria();
l_segmento *find_segmento_lista(char* idSegmento, List *segmentos);
l_frame *inicializarFrame(int cantidad, char* plato);
void limpiarMemoria();
void iniciarMemoria();
void *frameLibre();
void imprimirBitMap();
List tablaRestaurantes;
List tablaFrames;
char* punteroBitMap;
t_bitarray *bitMap;

l_pagina* plato_en_pagina(char* plato, List* lista);

void agregar_plato_pedido(l_pagina* pagina, int cantidad);
void confirmar_pedido_segmento(l_segmento *segmento);