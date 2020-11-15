#include "planificacion.h"



void inicializar_colas()
{
    inicializar_colas_ready_exec();
    inicializar_colas_io();
}

int inicializar_colas_ready_exec(){

    for(int i=0;i<cantidad_cocineros;i++){

        t_ready* cola_ready = malloc(sizeof(t_ready));
        t_exec* exec = malloc(sizeof(t_exec));
        exec->ocupado = 0;
        exec->plato = NULL;

        if(!isemptylist(afinidades)){
            char* afinidad = popfrontlist(&afinidades);
            cola_ready->afinidad = afinidad;
        }else{
            cola_ready->afinidad = "GENERAL";
        }

        initlist(cola_ready->platos_espera);
        cola_ready->puntero_exec = exec;

        pushbacklist(&colas_ready,cola_ready);

    }

    return 1;
}

void inicializar_colas_io(){

    for(int i=0;i<cantidad_hornos;i++){

        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->plato = NULL;

        pushbacklist(&colas_hornos,horno);

    }


}

int ocupar_horno(t_pcb* pcb){

    for(IteratorList iter_horno = beginlist(colas_hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){

        t_horno* horno = iter_horno->data;

        if(horno->ocupado == 0){
            horno->ocupado = 1;
            horno->plato = pcb->plato;
            pcb->estado = BLOCKED;
            return 1;

        }

    }

    return 0;

}



int ocupar_exec(t_pcb* pcb){

    t_exec* puntero_exec = pcb->cola_ready_perteneciente->puntero_exec;

    if(puntero_exec == NULL){
        puntero_exec->plato = pcb->plato;
        pcb-> estado = EXEC;

    }

    return 0;

}


int paso_ready(t_pcb* pcb){   
    
   return 0;

}


int paso_block(t_pcb* pcb)
{
    return 1;
}

int paso_exec(t_pcb* pcb){
    return 1;
};

int paso_exit(t_pcb* pcb){
    return 1;
};

t_ready* asignar_cola_ready(t_plato* plato){

    for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* ready = iter_ready->data;

        if(!strcmp(plato->nombre, ready->afinidad)){
            pushbacklist(ready->platos_espera,plato);
            return ready;

        }

    }

    for(IteratorList iter_ready2 = beginlist(colas_ready); iter_ready2 != NULL; iter_ready2 = nextlist(iter_ready2)){
        t_ready* ready2 = iter_ready2->data;

        if(!strcmp(ready2->afinidad,"GENERAL")){
            pushbacklist(ready2->platos_espera,plato);
            return ready2;

        }

    }

    return NULL;
    

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
    plato->pasos = pasos;
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
 

    return pcb;
}

t_pedido* creacion_pedido(int id_pedido, List* platos){

    t_pedido* pedido = malloc(sizeof(t_pedido));
    pedido->id_pedido = id_pedido;
    pedido->platos = *platos;


    return pedido;

}


