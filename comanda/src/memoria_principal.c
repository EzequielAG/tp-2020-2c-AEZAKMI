#include "memoria_principal.h"

void iniciarMemoria(){

    puntero_memoria_principal = malloc(comanda_config->tamanio_memoria);

    //crearSegmento();

}

l_proceso *crearProceso(){

	l_proceso *resto = malloc(sizeof(l_proceso));

	List tablaSegmentos;
	initlist(&tablaSegmentos);

    resto->punteroTablaSegmentos = &tablaSegmentos;

	return resto;
}


l_segmento *crearSegmento(){

	l_segmento *segmento = malloc(sizeof(l_segmento));

	List tablaPaginas;
	initlist(&tablaPaginas);

    segmento->punteroTablaPaginas = &tablaPaginas;

	return segmento;
}


void crear_pagina(l_segmento *segmento, void* puntero){
	l_pagina *pagina = malloc(32);
	pagina->bitPresencia = 0;
	//pagina->frame = puntero;
	pushbacklist(segmento->punteroTablaPaginas, pagina);
}

