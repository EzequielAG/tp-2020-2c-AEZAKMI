#include "memoria_principal.h"

void iniciarMemoria(){

}

l_proceso *crearProceso(char *nombreResto){

    l_proceso *resto = malloc(sizeof(l_proceso));

    List *tablaSegmentos = malloc(sizeof(List));
    initlist(tablaSegmentos);

    resto->nombreResto = nombreResto;
    resto->punteroTablaSegmentos = tablaSegmentos;

    return resto;
}


void crearSegmento(l_proceso *resto, char *idPedido){

    l_segmento *segmento = malloc(sizeof(l_segmento));

    List *tablaPaginas = malloc(sizeof(List));
    initlist(tablaPaginas);

    segmento->idPedido = idPedido;
    segmento->punteroTablaPaginas = tablaPaginas;

    pushbacklist(resto->punteroTablaSegmentos, segmento);
}


void crear_pagina(l_segmento *segmento, int cantidad, char *plato){
    l_pagina *pagina = malloc(sizeof(l_pagina));
    pagina->bitPresencia = 1;

	pagina->frame = inicializarFrame(cantidad, plato);

    pushbacklist(segmento->punteroTablaPaginas, pagina);
}

l_frame *inicializarFrame(int cantidad, char* plato){

	l_frame *frame = malloc(32);

	strncpy(frame->plato, plato, 24);
    frame->cantidadPlato = cantidad;
	frame->cantidadLista = 0;

	return frame;
}

void terminarPlatoPagina(l_pagina *pagina){

	pagina->frame->cantidadLista += 1; //FALTA CONDICION POR SI NO QUEDAN PLATOS EN COLA

}

l_proceso *find_resto_lista(char* nombreRestaurante){

    l_proceso *resto = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);

        if(!strcmp(nombreRestaurante, resto->nombreResto)){
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

void limpiarMemoria(){

	l_proceso *resto = NULL;
	l_segmento *segmento = NULL;
	l_pagina *pagina = NULL;

	//List *listaSegmentos = tablaRestaurantes->punteroTablaSegmentos;
	//List *listaPaginas = tablaRestaurantes->punteroTablaSegmentos->punteroTablaPaginas;

	IteratorList iterador = NULL;
	IteratorList jterador = NULL;
	IteratorList kterador = NULL;

    for(iterador = beginlist(tablaRestaurantes);iterador!=NULL;iterador = nextlist(iterador)){
        resto = dataiterlist(iterador);


		for(jterador = beginlist(*resto->punteroTablaSegmentos);jterador!=NULL;jterador = nextlist(jterador)){
        	segmento = dataiterlist(jterador);


			for(kterador = beginlist(*segmento->punteroTablaPaginas);kterador!=NULL;kterador = nextlist(kterador)){
        		pagina = dataiterlist(kterador);

				free(pagina->frame);

    		}

			emptylist(segmento->punteroTablaPaginas);
    	
		}

		emptylist(resto->punteroTablaSegmentos);

    }


}


void imprimirMemoria(){

	l_proceso *resto = NULL;
	l_segmento *segmento = NULL;
	l_pagina *pagina = NULL;

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

				printf("\t\tPlato: %s | Cantidad: %i | CantLista: %i\n", pagina->frame->plato, pagina->frame->cantidadPlato, pagina->frame->cantidadLista);

    		}
    	
		}

    }

	printf("---------------------------------------------------\n");

}