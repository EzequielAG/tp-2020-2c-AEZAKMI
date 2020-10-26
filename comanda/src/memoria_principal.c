#include "memoria_principal.h"

void iniciarMemoria(){

	void* puntero = NULL;

	puntero = puntero_memoria_principal;

	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		bitarray_clean_bit(bitMap, i);

	}

    while(puntero + 1024 > puntero_memoria_principal){
		
		puntero_memoria_principal += 32;
		
		pushbacklist(&tablaFrames, puntero_memoria_principal);

	}
}

l_proceso *crearProceso(char *nombreResto){

    l_proceso *resto = malloc(sizeof(l_proceso));
	resto->nombreResto = malloc(sizeof(char*));

    List *tablaSegmentos = malloc(sizeof(List));
    initlist(tablaSegmentos);

 //   resto->nombreResto = nombreResto;
	strcpy(resto->nombreResto,nombreResto);

    resto->punteroTablaSegmentos = tablaSegmentos;

    return resto;
}


int crearSegmento(l_proceso *resto, char *idPedido){

    l_segmento *segmento = malloc(sizeof(l_segmento));
	segmento->idPedido = malloc(sizeof(char*));


    List *tablaPaginas = malloc(sizeof(List));

    initlist(tablaPaginas);

  //  segmento->idPedido = idPedido;

	strcpy(segmento->idPedido,idPedido);
    segmento->punteroTablaPaginas = tablaPaginas;
	segmento->estadoPedido = 0;

    return pushbacklist(resto->punteroTablaSegmentos, segmento);
}


void crear_pagina(l_segmento *segmento, int cantidad, char *plato){
    l_pagina *pagina = malloc(sizeof(l_pagina));

	l_frame *frame;

	pagina->bitUso = 0;
    pagina->bitPresencia = 1;

	pagina->frame = frameLibre();

	frame = pagina->frame;
	frame->cantidadPlato = cantidad;
	frame->cantidadLista = 0;
	strncpy(frame->plato, plato, 24);


    pushbacklist(segmento->punteroTablaPaginas, pagina);
}

void terminarPlatoPagina(l_pagina *pagina){

	l_frame *frame = pagina->frame;

	frame->cantidadLista += 1;

	//pagina->frame->cantidadLista += 1; //FALTA CONDICION POR SI NO QUEDAN PLATOS EN COLA

}


int platos_listos(l_segmento* segmento){

	l_frame* frame = NULL;
	l_pagina* pagina = NULL;
	IteratorList iterador = NULL;

    for(iterador = beginlist(*(segmento->punteroTablaPaginas));iterador!=NULL;iterador = nextlist(iterador)){
        pagina = dataiterlist(iterador);
		frame = pagina->frame;

		if(frame->cantidadLista != frame->cantidadPlato){
			return 0;
		}

       
	}
	

    return 1;

}

void agregar_plato_pedido(l_pagina* pagina, int cantidad){
	l_frame* frame = pagina->frame;
	frame->cantidadPlato += cantidad;

}

void confirmar_pedido_segmento(l_segmento *segmento){
    segmento->estadoPedido = 1;
}

l_proceso *find_resto_lista(char* nombreRestaurante){

    l_proceso *resto = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);

        if(strcmp(nombreRestaurante, resto->nombreResto) == 0){
            return resto;
        }
    }
     return NULL;

}

l_segmento *find_segmento_lista(char* idSegmento, List *segmentos){

    l_segmento *seg = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(*segmentos);iterador!=NULL;iterador = nextlist(iterador)){
        seg = dataiterlist(iterador);

        if(!strcmp(idSegmento, seg->idPedido)){
            return seg;
        }
    }
    return NULL;

}

l_pagina* plato_en_pagina(char* plato, List* lista){

	l_pagina *pagina = NULL;
	l_frame* frame = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(*lista);iterador!=NULL;iterador = nextlist(iterador)){
        pagina = dataiterlist(iterador);
		frame = pagina->frame;

        if(!strcmp(plato,frame->plato)){
            return pagina;
        }
    }
    return NULL;
}

void imprimirMemoria(){

	l_proceso *resto = NULL;
	l_segmento *segmento = NULL;
	l_pagina *pagina = NULL;
	l_frame *frame = NULL;

	//List *listaSegmentos = tablaRestaurantes->punteroTablaSegmentos;
	//List *listaPaginas = tablaRestaurantes->punteroTablaSegmentos->punteroTablaPaginas;

	IteratorList iterador = NULL;
	IteratorList jterador = NULL;
	IteratorList kterador = NULL;

	printf("---------------------------------------------------\n");

    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);

		printf("Restaurant: %s | Contiene los segmentos: \n", resto->nombreResto);

		for(jterador = beginlist(*resto->punteroTablaSegmentos);jterador!=NULL;jterador = nextlist(jterador)){
        	segmento = dataiterlist(jterador);

			printf("\tID: %s | Contiene las paginas: \n", segmento->idPedido);

			for(kterador = beginlist(*segmento->punteroTablaPaginas);kterador!=NULL;kterador = nextlist(kterador)){
        		pagina = dataiterlist(kterador);

				frame = pagina->frame;

				printf("\t\tPlato: %s | Cantidad: %i | CantLista: %i\n", frame->plato, frame->cantidadPlato, frame->cantidadLista);

    		}
    	
		}

    }

	printf("---------------------------------------------------\n");

}

void *frameLibre(){

    for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		int bitOcupado = bitarray_test_bit(bitMap, i);
		void *punteroFrame = atlist(tablaFrames, i);

		if(!bitOcupado){
			bitarray_set_bit(bitMap, i);
			return punteroFrame;
		}

	}

	return NULL;

}

void imprimirBitMap(){

	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		void* puntero = atlist(tablaFrames, i);

		int bitOcupado = bitarray_test_bit(bitMap, i);
		
		printf("\nBit: %d | Posicion: %d | Direccion de memoria: %p\n", bitOcupado, i, puntero);
	}

}