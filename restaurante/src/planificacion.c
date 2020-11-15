#include "planificacion.h"

void inicializar_colas()
{
    inicializar_colas_ready_exec();
    inicializar_colas_io();
}

int inicializar_colas_ready_exec(){

    for(int i=0;i<cantidad_cocineros;i++){

        t_ready* cola_ready = malloc(sizeof(t_ready));
       
       
        if(!isemptylist(afinidades)){
            char* afinidad = popfrontlist(&afinidades);
            cola_ready->afinidad = afinidad;
        }else{
            cola_ready->afinidad = "GENERAL";
        }

       
        initlist(&cola_ready->platos_espera);
        cola_ready->puntero_exec = crear_exec();

        pushbacklist(&colas_ready,cola_ready);

    }

    return 1;
}


t_exec* crear_exec(){

    t_exec* exec = malloc(sizeof(t_exec));
    exec->ocupado = 0;
    exec->plato = NULL;

    return exec;
}

void inicializar_colas_io(){

    for(int i=0;i<cantidad_hornos;i++){

        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->plato = NULL;

        pushbacklist(&cola_io->hornos,horno);

    }

}

t_horno* paso_block(t_pcb* pcb){

    for(IteratorList iter_horno = beginlist(cola_io->hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){

        t_horno* horno = iter_horno->data;

        if(horno->ocupado == 0){
            horno->ocupado = 1;
            horno->plato = pcb->plato;
            pcb->estado = BLOCKED;
            printf(" - El plato %s esta en el horno, su estado es: %s \n",pcb->plato->nombre,obtener_estado(pcb->estado));
         
            return horno;

        }

    }

    // TODO: CHEQUEAR REPOSAR

    pushbacklist(&cola_io->platos_espera,pcb->plato);
    pcb->estado = BLOCKED_SUSPENDED;
    printf(" - El plato %s esta en lista de espera para horno, su estado es: %s \n",pcb->plato->nombre,obtener_estado(pcb->estado));
   

    return NULL;

}



int paso_exec(t_pcb* pcb){

    t_ready* cola_ready = pcb->cola_ready_perteneciente;

    if(cola_ready->puntero_exec->ocupado == 0){
        cola_ready->puntero_exec->plato = pcb->plato;
        cola_ready->puntero_exec->ocupado = 1;
        pcb->estado = EXEC;
        printf(" - El plato %s esta ejecutandose por un cocinero de afinidad %s, su estado es: %s \n",pcb->plato->nombre, cola_ready->afinidad,obtener_estado(pcb->estado));
        return 1;
    }

       printf(" - El plato %s no pudo ser ejecutado, su estado es: %s \n",pcb->plato->nombre,obtener_estado(pcb->estado));
       return 0;

}

char* obtener_estado(int estado){

    if(estado == EXEC){
        return "EXEC";
    } else if(estado == BLOCKED){
        return "BLOCKED";
    } else if(estado == BLOCKED_SUSPENDED){
        return "BLOCKED_SUSPENDED";
    } else if(estado == EXIT){
        return "EXIT";
    } else if(estado == READY){
        return "READY";
    }

    return NULL;


}

int paso_ready(t_pcb* pcb){   

    pushbacklist(&pcb->cola_ready_perteneciente->platos_espera,pcb->plato);
    pcb->estado = READY;
    printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));
    
   return 1;

}


int paso_exit(t_pcb* pcb){

    pushbacklist(&colas_exit,pcb->plato);
    pcb->estado = EXIT;

    printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));
     
    return 1;
};


int termino_pedido(int id_pedido){

    for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        
        t_pcb* pcb = iter_pcb->data;

        if(pcb->id_pedido == id_pedido && pcb->estado == EXIT){
            return 1;
        }

    }

    return 0;

}

int pasos_ejecutados(t_pcb* pcb){

    for(IteratorList iter_pasos = beginlist(pcb->plato->pasos); iter_pasos != NULL; iter_pasos = nextlist(iter_pasos)){
        t_paso* paso = iter_pasos->data;

        if(paso->se_ejecuto == 0){
            return 0;
        }
        
    }

    return 1;
}


t_ready* asignar_cola_ready(t_plato* plato){

    for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* ready = iter_ready->data;

        if(!strcmp(plato->nombre, ready->afinidad)){
            pushbacklist(&ready->platos_espera,plato);
            return ready;

        }

    }

    for(IteratorList iter_ready2 = beginlist(colas_ready); iter_ready2 != NULL; iter_ready2 = nextlist(iter_ready2)){
        t_ready* ready2 = iter_ready2->data;

        if(!strcmp(ready2->afinidad,"GENERAL")){
            pushbacklist(&ready2->platos_espera,plato);
            return ready2;

        }

    }

    return NULL;
    

}

int ejecutar_ciclo(t_pcb* pcb, t_paso* paso){

    if(paso->se_ejecuto == 0){

        if(pcb->estado == READY){
        
            if(es_paso_io(paso)){
                t_horno* respuesta_block = paso_block(pcb); 
                
                if(respuesta_block != NULL){
                    sleep((paso->ciclo_cpu)*1); //REVISAR
                    paso->se_ejecuto = 1; 
                    respuesta_block->ocupado = 0;

                    //CHEQUEAR SI HAY ALGO EN LA COLA DE ESPERA DE HORNO, EN CASO QUE HAYA ASIGNARLO
                    paso_ready(pcb);
                }
                
            }else{
                int respuesta_paso = paso_exec(pcb);
                
                if(respuesta_paso){
                    sleep((paso->ciclo_cpu)*1);
                    paso->se_ejecuto = 1;
                    pcb->cola_ready_perteneciente->puntero_exec->ocupado = 0; 
                    paso_ready(pcb); //CHEQUEAR
                }
            }
        
        }

        if(pcb->estado == BLOCKED_SUSPENDED){

            if(horno_libre()){

            t_horno* respuesta_block_suspendido = paso_block(pcb);
                    
                if(respuesta_block_suspendido != NULL){
                    sleep((paso->ciclo_cpu)*1); //REVISAR
                    paso->se_ejecuto = 1; 
                    respuesta_block_suspendido->ocupado = 0;
                }

            }
        }

        
    }


    if(pasos_ejecutados(pcb)){
        paso_exit(pcb);
        return 1;
    };
    
    return ejecutar_ciclo(pcb,paso);

}


void planificacion_fifo(){ // REVISAR CANTIDAD TOTAL Y CANTIDAD LISTA


    for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        t_pcb* pcb = iter_pcb->data;
        
        ejecutar_ciclo(pcb,NULL);
       
    }

      
}

t_paso* paso_a_ejecutar(t_pcb* pcb){

    for(IteratorList iter_pasos = beginlist(pcb->plato->pasos); iter_pasos != NULL; iter_pasos = nextlist(iter_pasos)){
        t_paso* paso = iter_pasos->data;
        if(paso->se_ejecuto == 0){
            return paso;
        }
        
    }

    return NULL;
}



List* hilos_ready(){
    List* lista_hilos_ready = NULL;
    for(int i = 0; i<cantidad_cocineros;i++){
        // pthread_t hilo;
        // pthread_create(&hilo,NULL,(void*)NULL, NULL);
        // pushbacklist(lista_hilos_ready,hilo);

    }
    return lista_hilos_ready;
}


int horno_libre(){

    for(IteratorList iter_io = beginlist(cola_io->hornos); iter_io != NULL; iter_io = nextlist(iter_io)){
        t_horno* horno = iter_io->data;
        
        if(horno-> ocupado == 0){
            return 1;
        }
    }

    return 0;
}


int es_paso_io(t_paso* paso){

    if( !strcmp((paso->nombre_paso),"HORNEAR") || !strcmp((paso->nombre_paso),"Hornear")){
        return 1;
    }

   return 0;
}


t_paso* crear_paso(char* nombre_paso, int ciclo_cpu){

    t_paso* paso = malloc(sizeof(paso));
     
    paso->nombre_paso = nombre_paso;
    paso->ciclo_cpu = ciclo_cpu;
    paso->se_ejecuto = 0;


    return paso;

}


t_plato* crear_plato(char* nombre, List* pasos, int pedido_id, int cantidad_total, int cantidad_listo){

    t_plato* plato = malloc(sizeof(t_plato));

    plato->nombre = nombre;
    plato->pasos = *pasos;
    plato->cantidad_total = cantidad_total;
    plato->cantidad_listo = cantidad_listo;

    crear_pcb(pedido_id,READY,plato);


    return plato;

}


t_pcb* crear_pcb(int id_pedido, int estado,t_plato* plato){ //AGREGAR PID

    t_pcb* pcb = malloc(sizeof(t_pcb));
 
    pcb->id_pedido = id_pedido;
    pcb->estado = estado;
    pcb->plato = plato;
    pcb->cola_ready_perteneciente = asignar_cola_ready(plato);

    pushfrontlist(&colas_pcb,pcb);

    return pcb;
}

t_pedido* creacion_pedido(int id_pedido, List* platos){

    t_pedido* pedido = malloc(sizeof(t_pedido));
    pedido->id_pedido = id_pedido;
    pedido->platos = *platos;


    return pedido;

}


