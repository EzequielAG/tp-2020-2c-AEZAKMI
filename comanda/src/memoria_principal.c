#include "memoria_principal.h"

void iniciarMemoria(){

	void* puntero = NULL;
	numeroPaginaGlobal = 0;

	puntero = puntero_memoria_principal;


	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		bitarray_clean_bit(bitMap, i);

	}

    while(puntero + tamanioMemoria > puntero_memoria_principal){
		
		pushbacklist(&tablaFrames, puntero_memoria_principal);

		puntero_memoria_principal += 32;

	}

	puntero_memoria_principal = puntero;

	//iniciarMemoriaSwap();
}

l_proceso *crearProceso(char *nombreResto){

    l_proceso *resto = malloc(sizeof(l_proceso));
	resto->nombreResto = malloc(strlen(nombreResto)+1);

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

void crear_pagina2(l_segmento *segmento, int cantidad, char *plato){
    l_pagina *pagina = malloc(sizeof(l_pagina));

	l_frame *frame;

	pagina->numPagina = numeroPaginaGlobal;

	pagina->bitUso = 0;
    pagina->bitPresencia = 0;
	pagina->bitModificado = 0;

	pagina->frame = NULL;
	pagina->swap = frameLibreSwap();

	frame = pagina->swap;
	frame->cantidadPlato = cantidad;
	frame->cantidadLista = 0;
	strncpy(frame->plato, plato, 24);

	modificarPagina(pagina);

    pushbacklist(segmento->punteroTablaPaginas, pagina);

	numeroPaginaGlobal += 1;
}

void terminarPlatoPagina(l_pagina *pagina){

	l_frame *frame = pagina->frame;

	frame->cantidadLista += 1;

	pagina->bitModificado = 1;

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

	pagina->bitModificado = 1;

}

void confirmar_pedido_segmento(l_segmento *segmento){
    segmento->estadoPedido = 1;
}

void terminar_pedido_segmento(l_segmento *segmento){
	segmento->estadoPedido = 2;
}

void eliminarSegmento(l_proceso *resto, l_segmento* segmento){

	l_pagina *pagina = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(*segmento->punteroTablaPaginas);iterador!=NULL;iterador = nextlist(iterador)){
        pagina = dataiterlist(iterador);

		quitarSiExiste(pagina);

		desalojarDePrincipal(pagina);

		//free(pagina);
		
    }

	emptylist(segmento->punteroTablaPaginas);
	free(segmento->punteroTablaPaginas);

	desalojarPedido(resto, segmento);

	free(segmento->idPedido);

	free(segmento);
	

}

void desalojarPedido(l_proceso *resto, l_segmento* segmento){

	l_segmento* segmentoEncontrado;
	IteratorList iterador = NULL;
	List *lista = resto->punteroTablaSegmentos;

    for(iterador = beginlist(*lista);iterador!=NULL;iterador = nextlist(iterador)){
        segmentoEncontrado = dataiterlist(iterador);

        if(!strcmp(segmentoEncontrado->idPedido, segmento->idPedido)){
            popiterlist(resto->punteroTablaSegmentos, iterador);
        }
    }


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
	l_frame* swap = NULL;

    IteratorList iterador = NULL;

    for(iterador = beginlist(*lista);iterador!=NULL;iterador = nextlist(iterador)){
        pagina = dataiterlist(iterador);
		frame = pagina->frame;
		swap = pagina->swap;

		if(!strcmp(plato,swap->plato)){
			if(frame != NULL){
            	return pagina;
			}
        }
        
    }
    return NULL;
}

void pasarPaginasAPrincipal(l_segmento *segmento){

	l_pagina *pagina = NULL;
	IteratorList iterador = NULL;

	for(iterador = beginlist(*segmento->punteroTablaPaginas);iterador!=NULL;iterador = nextlist(iterador)){

		pagina = dataiterlist(iterador);

		modificarPagina(pagina);

	}

}

void *frameLibre(){

    for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		int bitOcupado = bitarray_test_bit(bitMap, i);
		void *punteroFrame = atlist(tablaFrames, i);

		if(!bitOcupado){
			return punteroFrame;
		}

	}

	return NULL;

}

void ocuparFrame(void* frame){

	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		void *punteroFrame = atlist(tablaFrames, i);

		if(punteroFrame == frame){
			bitarray_set_bit(bitMap, i);
			return;
		}

	}

}

void desocuparFrame(void* frame){
	
	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		void *punteroFrame = atlist(tablaFrames, i);

		if(punteroFrame == frame){
			bitarray_clean_bit(bitMap, i);
			return;
		}

	}
}

void *frameLibreSwap(){
	
	for(int i=0; bitarray_get_max_bit(bitMapSwap) > i; i++){

		int bitOcupado = bitarray_test_bit(bitMapSwap, i);
		void *punteroSwap = atlist(tablaSwap, i);

		if(!bitOcupado){
			bitarray_set_bit(bitMapSwap, i);
			return punteroSwap;
		}

	}

	return NULL;
}

void modificarPagina(l_pagina* paginaSwap){

	int resultadoDePagAPrincipal = pasarAPrincipal(paginaSwap);
	if(!resultadoDePagAPrincipal){
		printf("No se paso la pagina de swap a memoria principal correctamente");
		return;
	}

	paginaSwap->bitUso = 1;

	quitarSiExiste(paginaSwap);

	pushfrontlist(&pilaPaginasAlgoritmos, paginaSwap);

}

void quitarSiExiste(l_pagina* paginaSwap){

	l_pagina* pagina = NULL;
	IteratorList iterador = NULL;

	for(iterador = beginlist(pilaPaginasAlgoritmos);iterador!=NULL;iterador = nextlist(iterador)){
	
		pagina = dataiterlist(iterador);

		if(paginaSwap->numPagina == pagina->numPagina){
			popiterlist(&pilaPaginasAlgoritmos, iterador);
		}
	
	}

}

int pasarAPrincipal(l_pagina* paginaSwap){

	if(paginaSwap->frame != NULL){
		return 1;
	}

	void* espacioLibre = ejecutarAlgoritmo();

	if(espacioLibre != NULL){
		
		ocuparFrame(espacioLibre);
		memcpy(espacioLibre, paginaSwap->swap, 32);
		paginaSwap->frame = espacioLibre;
    	paginaSwap->bitPresencia = 1;
		return 1;
	}

	return 0;

}

void *ejecutarAlgoritmo(){

	void* framelibre = frameLibre();

	if(framelibre != NULL) 
		return framelibre;
	printf("Ejecutando algoritmo: %s\n", algoritmo);
	if(!strcmp(algoritmo, "LRU")) 
		return ejecutarLRU();
	
	if(!strcmp(algoritmo, "CLOCK_MEJ"))
		return ejecutarClockMej();
	
	printf("ALGORITMO INVALIDO\n");

	return NULL;
}

void *ejecutarLRU(){

	l_pagina* pagina = popbacklist(&pilaPaginasAlgoritmos);

	return desalojarDePrincipal(pagina);

}

void *ejecutarClockMej(){
	
	l_pagina* pagina = NULL;
	IteratorList iterador = NULL;
	int gradoDeBusqueda = 0; //0 = bitUso en 0 + bitModificado en 0 | 1 = bitUso en 0 + bitModificado en 1

	iterador = beginlist(pilaPaginasAlgoritmos);

	while(iterador != NULL){
	
		pagina = dataiterlist(iterador);

		if(gradoDeBusqueda){
			if(!pagina->bitUso){
				popiterlist(&pilaPaginasAlgoritmos, iterador);
				return desalojarDePrincipal(pagina);
			} else {
				pagina->bitUso = 0;
			}
		}

		if(!gradoDeBusqueda){
			if(!pagina->bitUso){
				if(!pagina->bitModificado){
					popiterlist(&pilaPaginasAlgoritmos, iterador);
					return desalojarDePrincipal(pagina);
				}
			}
		}

		iterador = nextlist(iterador);

		if(iterador == NULL){
			iterador = beginlist(pilaPaginasAlgoritmos);
			gradoDeBusqueda = !gradoDeBusqueda;
		}
	
	}
	printf("\nError ejecutando el algoritmo!!\n");
	return NULL;
}

void *desalojarDePrincipal(l_pagina* pagina){

	void* direccionPagina = NULL;

	pagina->bitPresencia = 0;
	direccionPagina = pagina->frame;
	memcpy(pagina->swap, pagina->frame, 32);
	pagina->frame = NULL;
	desocuparFrame(direccionPagina);

	return direccionPagina;

}

void imprimirTodo(){

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
				if(frame != NULL){
					printf("\t\tPlato: %s | Cantidad: %i | CantLista: %i | Direccion Memoria: %p | Direccion Swap: %p\n", frame->plato, frame->cantidadPlato, frame->cantidadLista, pagina->frame, pagina->swap);
				}
    		}
    	
		}

    }

	printf("---------------------------------------------------\n");

}

void imprimirBitMap(){

	printf("\n---------------BIT MAP---------------\n\n");

	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		void* puntero = atlist(tablaFrames, i);

		int bitOcupado = bitarray_test_bit(bitMap, i);
		
		printf("Bit: %d | Posicion: %d | Direccion de memoria: %p\n", bitOcupado, i, puntero);
	}

}

void imprimirMemoria(){

	l_frame *frame = NULL;

	printf("\n-------------MEMORIA PRINCIPAL-------------\n\n");

	for(int i=0; bitarray_get_max_bit(bitMap) > i; i++){

		int bitOcupado = bitarray_test_bit(bitMap, i);
		frame = atlist(tablaFrames, i);

		if(bitOcupado){
			printf("Plato: %s | Cantidad: %i | CantLista: %i | Direccion Memoria: %p\n", frame->plato, frame->cantidadPlato, frame->cantidadLista, frame);
		}

	}

}

void imprimirSwap(){

	puntero_memoria_swap = frontlist(tablaSwap);

	//l_frame *frame = malloc(32);
	char frame[2050];
	int bytesLeidos;

	printf("\n-------------MEMORIA SWAP-------------\n\n");

	lseek(archivoSwap, 0, SEEK_SET);

	//printf("Valor archivo: %d | ", archivoSwap);

	bytesLeidos = read(archivoSwap, frame, 2048);

	printf("BytesLeidos: %d | Direccion Swap: %p\n", bytesLeidos,  puntero_memoria_swap);


}