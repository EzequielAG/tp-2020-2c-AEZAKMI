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

int paso_block(t_pcb* pcb){

    for(IteratorList iter_horno = beginlist(cola_io->hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){

        t_horno* horno = iter_horno->data;

        if(horno->ocupado == 0){
            horno->ocupado = 1;
            horno->plato = pcb->plato;
            pcb->estado = BLOCKED;
         
            return 1;

        }

    }

    pushbacklist(&cola_io->platos_espera,pcb->plato);
    pcb->estado = BLOCKED_SUSPENDED;

    return 0;

}



int paso_exec(t_pcb* pcb){

    t_ready* cola_ready = pcb->cola_ready_perteneciente;

    if(cola_ready->puntero_exec->ocupado == 0){
        cola_ready->puntero_exec->plato = pcb->plato;
        cola_ready->puntero_exec->ocupado = 1;
        pcb->estado = EXEC;
        return 1;
    }
       
    return 0;

}


int paso_ready(t_pcb* pcb){   

    pushbacklist(&pcb->cola_ready_perteneciente->platos_espera,pcb->plato);
    pcb->estado = READY;
    
   return 1;

}


int paso_exit(t_pcb* pcb){

    pushbacklist(&colas_exit,pcb->plato);
    pcb->estado = EXIT;

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

        if(paso->se_ejecuto == 1){
            return 1;
        }
        
    }

    return 0;
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

int ejecutar_ciclo(t_pcb* pcb){

    for(IteratorList iter_paso = beginlist(pcb->plato->pasos); iter_paso != NULL; iter_paso = nextlist(iter_paso)){
        t_paso* paso = iter_paso->data;

        if(paso->se_ejecuto == 0){

            if(pcb->estado == READY){
            
                if(es_paso_io(paso)){
                    int respuesta_block = paso_block(pcb);
                    if(respuesta_block){
                        sleep((paso->ciclo_cpu)*1); //REVISAR
                        paso->se_ejecuto = 1; 
                    }
                    
                }else{
                    int respuesta_paso = paso_exec(pcb);
                    
                    if(respuesta_paso){
                        sleep((paso->ciclo_cpu)*1);
                        paso->se_ejecuto = 1; 
                    }
                }
           
           }

           if(pcb->estado == BLOCKED_SUSPENDED){
                int respuesta_block_suspendido = paso_block(pcb);
                if(respuesta_block_suspendido){
                        sleep((paso->ciclo_cpu)*1); //REVISAR
                        paso->se_ejecuto = 1; 
                    }
           }
           
        }

    }

    if(pcb->estado == READY){
        ejecutar_ciclo(pcb);
    }

    if(pasos_ejecutados(pcb)){
        paso_exit(pcb);
        return 1;
    };

    return 0;

}

void planificacion_fifo(){


    for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        t_pcb* pcb = iter_pcb->data;
        
        if(!termino_pedido(pcb->id_pedido)){
            ejecutar_ciclo(pcb);
        }

    }

      
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


t_pcb* crear_pcb(int id_pedido, int estado,t_plato* plato){
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


