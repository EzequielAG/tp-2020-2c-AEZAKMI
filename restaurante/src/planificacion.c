#include "planificacion.h"

void inicializar_colas()
{
    inicializar_colas_ready();
    inicializar_colas_io();
    inicializar_colas_exec();
   
    printf("\n \n");
    printf("Las colas de ready creadas son: \n");
    for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* cola_ready = iter_ready->data;
        printf("- %s \n",cola_ready->afinidad);


        for(IteratorList iter_espera = beginlist(cola_ready->pcb_espera); iter_espera != NULL; iter_espera = nextlist(iter_espera)){
            t_pcb* pcb = iter_espera->data;

            printf("Plato en espera %s \n",pcb->plato->nombre);

        }

    } printf("---------- \n");

    printf("La cantidad de hornos creadas es de: \n");
    for(IteratorList iter_horno = beginlist(hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){
        t_horno* horno = iter_horno->data;
        printf("- %d \n", horno->ocupado);

    }printf("---------- \n");

    printf("Los cocineros tienen afinidad: \n");
    for(IteratorList iter_cocinero = beginlist(colas_exec); iter_cocinero != NULL; iter_cocinero = nextlist(iter_cocinero)){
        t_exec* cocinero = iter_cocinero->data;

        printf(" -%s \n",cocinero->afinidad);

        // pthread_create(cocinero->hilo_ejecucion_exec,NULL,(void*)paso_exec,cocinero);
        // pthread_detach(*cocinero->hilo_ejecucion_exec);
        //  pthread_join(*cocinero->hilo_ejecucion_exec,NULL);


    }printf("---------- \n");


  

}

void inicializar_colas_ready(){

    for(IteratorList iter_afinidades = beginlist(afinidades); iter_afinidades != NULL; iter_afinidades = nextlist(iter_afinidades)){
        char* afinidad_lista = iter_afinidades->data;

        if(!cola_ready_creada(afinidad_lista)){
            t_ready* cola_ready_nueva = malloc(sizeof(t_ready));
            cola_ready_nueva->afinidad = afinidad_lista;
            initlist(&cola_ready_nueva->pcb_espera);
            pushbacklist(&colas_ready,cola_ready_nueva);

        }
  }

    if(sizelist(afinidades)<cantidad_cocineros){
        t_ready* cola_ready_nueva = malloc(sizeof(t_ready));
        cola_ready_nueva->afinidad = "GENERAL";
        initlist(&cola_ready_nueva->pcb_espera);
        pushbacklist(&colas_ready,cola_ready_nueva);

    }
 
}

t_ready* cola_ready_cocinero(char* afinidad){

      for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* cola_ready = iter_ready->data;

        if(!strcmp(cola_ready->afinidad, afinidad)){
            return cola_ready;

        }
      }
        return NULL;
}



int cola_ready_creada(char* afinidad){

    for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* cola_ready = iter_ready->data;

        if(!strcmp(afinidad,cola_ready->afinidad)){
            return 1;
        }

  }

  return 0;
 
}

void inicializar_colas_exec(){
    
    for(int i=0; i<cantidad_cocineros;i++){
        
        if(!isemptylist(afinidades)){
            char* afinidad_lista = popfrontlist(&afinidades);
            t_ready* cola_ready_afinidad = cola_ready_cocinero(afinidad_lista);
            t_exec* cola_exec = crear_exec(cola_ready_afinidad);
            pushbacklist(&colas_exec, cola_exec);
        }else{
            t_ready* cola_ready_afinidad = cola_ready_cocinero("GENERAL");
            t_exec* cola_exec = crear_exec(cola_ready_afinidad);
            pushbacklist(&colas_exec, cola_exec);
        }
         

    }

}


void inicializar_colas_io(){

    for(int i=0;i<cantidad_hornos;i++){

        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->plato = NULL;

        pushbacklist(&hornos,horno);

    }

}




void* paso_exec(t_exec* cocinero){

    while(true){
    

    sleep(3);

    printf("PASO EL SEMAFOROOOO! \n");

        if(!isemptylist(cocinero->cola_ready->pcb_espera)){

            printf("PASO EL IF! \n");

            t_pcb* pcb_en_ejecucion = popfrontlist(&cocinero->cola_ready->pcb_espera);

            pcb_en_ejecucion->estado = EXEC;

            for(IteratorList iter_pasos = beginlist(pcb_en_ejecucion->plato->pasos); iter_pasos != NULL; iter_pasos = nextlist(iter_pasos)){
                t_paso* paso = iter_pasos->data;

                if(paso->se_ejecuto == 0){
                    
                    if(!es_paso_io(paso)){
                        sleep(paso->ciclo_cpu);
                        paso->se_ejecuto = 1;
                        printf(" EJECUTO EXEC \n");
                        break;
                    }else{
                        printf(" ES PASO IO BOLUDOOO \n");
                        break;
                    }

            }
        }

    }

    printf(" - El plato no pudo ser ejecutado, su estado es:\n");
    
    }
    return NULL;
}



int paso_ready(t_pcb* pcb){   

    t_ready* cola_ready = cola_ready_pcb(pcb);
    pushbacklist(&cola_ready->pcb_espera,pcb);
    pcb->estado = READY;
    printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));

    
   return 1;

}



int plato_general(char* nombre_plato){

    for(IteratorList iter_afinidad = beginlist(afinidades); iter_afinidad != NULL; iter_afinidad = nextlist(iter_afinidad)){
        char* afinidad = iter_afinidad->data;

        if(!strcmp(afinidad,nombre_plato)){
            return 0;
        }

    }

    return 1;
}







int paso_exit(t_pcb* pcb){

    pushbacklist(&colas_exit,pcb->plato);
    pcb->estado = EXIT;

    printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));
     
    return 1;
};


t_horno* paso_block(t_pcb* pcb){

    for(IteratorList iter_horno = beginlist(hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){

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

    pushbacklist(&pcb_espera_horno,pcb);
    pcb->estado = BLOCKED_SUSPENDED;
    printf(" - El plato %s esta en lista de espera para horno, su estado es: %s \n",pcb->plato->nombre,obtener_estado(pcb->estado));
   

    return NULL;

}



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


t_ready* cola_ready_pcb(t_pcb* pcb){

    for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
        t_ready* ready = iter_ready->data;

        if(!strcmp(pcb->plato->nombre, ready->afinidad)){

            return ready;

        }

    }

    for(IteratorList iter_ready2 = beginlist(colas_ready); iter_ready2 != NULL; iter_ready2 = nextlist(iter_ready2)){
        t_ready* ready2 = iter_ready2->data;

        if(!strcmp(ready2->afinidad,"GENERAL")){

            return ready2;

        }

    }

    return NULL;
    

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





int pedidos_finalizados(){

    int tamanio_colas_pcb = sizelist(colas_pcb);
    int i=0;
    for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        t_pcb* pcb = iter_pcb->data;

        if(pcb->estado == EXIT){
            i++;

        }

    }

    return (i==tamanio_colas_pcb);
}




int horno_libre(){

    for(IteratorList iter_io = beginlist(hornos); iter_io != NULL; iter_io = nextlist(iter_io)){
        t_horno* horno = iter_io->data;
        
        if(horno-> ocupado == 0){
            return 1;
        }
    }

    return 0;
}


int es_paso_io(t_paso* paso){

    if( !strcmp((paso->nombre_paso),"HORNEAR") || !strcmp((paso->nombre_paso),"Hornear") || !strcmp((paso->nombre_paso),"Reposar")){
        return 1;
    }

   return 0;
}



void ocupar_horno_libre(){

    if(!isemptylist(pcb_espera_horno)){
        t_plato* plato = popfrontlist(&pcb_espera_horno);
        for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
            t_pcb* pcb = iter_pcb->data;
            if(!strcmp(pcb->plato->nombre,plato->nombre)){
                paso_block(pcb);
            }
        }
    } 
}



void planificacion(){

    printf("Estoy en planificacion \n");

    for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        t_pcb* pcb = iter_pcb->data;

        if(pcb->estado == NEW){
            paso_ready(pcb);
        }


    }printf("---------- \n");



    for(IteratorList iter_cocinero = beginlist(colas_exec); iter_cocinero != NULL; iter_cocinero = nextlist(iter_cocinero)){
        t_exec* cocinero = iter_cocinero->data;

        printf(" -%s \n",cocinero->afinidad);

        pthread_create(cocinero->hilo_ejecucion_exec,NULL,(void*)paso_exec,cocinero);
        pthread_detach(*cocinero->hilo_ejecucion_exec);
        //pthread_join(*cocinero->hilo_ejecucion_exec,NULL);


    }printf("---------- \n");


}






t_paso* crear_paso(char* nombre_paso, int ciclo_cpu){

    t_paso* paso = malloc(sizeof(paso));
     
    paso->nombre_paso = nombre_paso;
    paso->ciclo_cpu = ciclo_cpu;
    paso->se_ejecuto = 0;


    return paso;

}


t_plato* crear_plato(char* nombre, List* pasos, int pedido_id, int cantidad_total, int cantidad_listo,int pid){

    t_plato* plato = malloc(sizeof(t_plato));

    plato->nombre = nombre;
    plato->pasos = *pasos;
    plato->cantidad_total = cantidad_total;
    plato->cantidad_listo = cantidad_listo;

    crear_pcb(pedido_id,pid,plato);


    return plato;

}


t_pcb* crear_pcb(int id_pedido,int pid,t_plato* plato){

    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->pid = pid;
    pcb->id_pedido = id_pedido;
    pcb->estado = NEW;
    pcb->plato = plato;    

    pushfrontlist(&colas_pcb,pcb);


    return pcb;
}

t_pedido* creacion_pedido(int id_pedido, List* platos){

    t_pedido* pedido = malloc(sizeof(t_pedido));
    pedido->id_pedido = id_pedido;
    pedido->platos = *platos;


    return pedido;

}


t_exec* crear_exec(t_ready* cola_ready){

    t_exec* exec = malloc(sizeof(t_exec));
    exec->ocupado = 0;
    exec->cola_ready = cola_ready;
    exec->afinidad = cola_ready->afinidad;
    exec->semaforo_exec = malloc(sizeof(sem_t));
    sem_init((exec->semaforo_exec),0,0);
    pthread_t* hilo_ejecucion_exe = malloc(sizeof(pthread_t));
    exec->hilo_ejecucion_exec = hilo_ejecucion_exe;
    // pthread_create(hilo_ejecucion_exe,NULL,(void*)paso_exec,exec);
    // pthread_detach(*hilo_ejecucion_exe);
    // pthread_join(*hilo_ejecucion_exe,NULL);

    return exec;
}
