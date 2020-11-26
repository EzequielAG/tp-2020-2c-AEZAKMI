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
        //printf("- %s \n",cola_ready->afinidad);


        // for(IteratorList iter_espera = beginlist(cola_ready->pcb_espera); iter_espera != NULL; iter_espera = nextlist(iter_espera)){
        //     t_pcb* pcb = iter_espera->data;

        //     printf("Plato en espera %s \n",pcb->plato->nombre);

        // }
        printf("Los cocineros tienen afinidad: \n");
        for(IteratorList iter_cocinero = beginlist(cola_ready->cocineros); iter_cocinero != NULL; iter_cocinero = nextlist(iter_cocinero)){
            t_exec* cocinero = iter_cocinero->data;

            printf(" %s \n",cocinero->afinidad);


        }

    } printf("---------- \n");

    printf("La cantidad de hornos creadas es de: \n");
    for(IteratorList iter_horno = beginlist(hornos); iter_horno != NULL; iter_horno = nextlist(iter_horno)){
        t_horno* horno = iter_horno->data;
        printf("- %d \n", horno->ocupado);

    }printf("---------- \n");

    


  

}

void iniciar_clock(){
    pthread_t thread;
    pthread_create(&thread,NULL,(void*)clock_cpu, NULL);
	pthread_detach(thread);
}

void clock_cpu(){
    
    while(true){
        sleep(2); //todo: pasar tiempo del config

        log_info(logger, "-----------CICLO-------------");

        for (IteratorList il = beginlist(suscriptores_cpu); il != NULL; il = nextlist(il)){
            sem_t* suscriptor = (sem_t*) dataiterlist(il);
            sem_post(suscriptor);
        }
    }
}


void inicializar_colas_ready(){

    for(IteratorList iter_afinidades = beginlist(afinidades); iter_afinidades != NULL; iter_afinidades = nextlist(iter_afinidades)){
        char* afinidad_lista = iter_afinidades->data;

        if(!cola_ready_creada(afinidad_lista)){
            t_ready* cola_ready_nueva = malloc(sizeof(t_ready));
            cola_ready_nueva->afinidad = afinidad_lista;
            initlist(&cola_ready_nueva->pcb_espera);
            initlist(&cola_ready_nueva->cocineros);
            cola_ready_nueva->sem_cocinero_libre = malloc(sizeof(sem_t));
            sem_init(cola_ready_nueva->sem_cocinero_libre, 0, 0);
            cola_ready_nueva->sem_pcb_espera = malloc(sizeof(sem_t));
            sem_init(cola_ready_nueva->sem_pcb_espera, 0, 0);
            char string_log[100];
            sprintf(string_log, "Se crea cola de ready de afinidad %s", cola_ready_nueva->afinidad);
            log_info(logger, string_log);
            pthread_t ejecuta_controlador_cola;
            pthread_create(&ejecuta_controlador_cola, NULL, (void*) controlador_ready, cola_ready_nueva);
            pthread_detach(ejecuta_controlador_cola);
            pushbacklist(&colas_ready,cola_ready_nueva);

        }
    }

    if(sizelist(afinidades)<cantidad_cocineros){
        t_ready* cola_ready_nueva = malloc(sizeof(t_ready));
        cola_ready_nueva->afinidad = "GENERAL";
        initlist(&cola_ready_nueva->pcb_espera);
        initlist(&cola_ready_nueva->cocineros);
        cola_ready_nueva->sem_cocinero_libre = malloc(sizeof(sem_t));
        sem_init(cola_ready_nueva->sem_cocinero_libre, 0, 0);
        cola_ready_nueva->sem_pcb_espera = malloc(sizeof(sem_t));
        sem_init(cola_ready_nueva->sem_pcb_espera, 0, 0);
        char string_log[100];
        sprintf(string_log, "Se crea cola de ready de afinidad GENERAL");
        log_info(logger, string_log);
        pthread_t ejecuta_controlador_cola;
        pthread_create(&ejecuta_controlador_cola, NULL, (void*) controlador_ready, cola_ready_nueva);
        pthread_detach(ejecuta_controlador_cola);
        pushbacklist(&colas_ready,cola_ready_nueva);

    }
 
}

// t_ready* cola_ready_cocinero(char* afinidad){

//       for(IteratorList iter_ready = beginlist(colas_ready); iter_ready != NULL; iter_ready = nextlist(iter_ready)){
//         t_ready* cola_ready = iter_ready->data;

//         if(!strcmp(cola_ready->afinidad, afinidad)){
//             return cola_ready;

//         }
//       }
//         return NULL;
// }



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
        
        if(atlist(afinidades,i) != NULL){
            char* afinidad_lista = atlist(afinidades,i);
            t_ready* cola = cola_por_afinidad(afinidad_lista);
            t_exec* cola_exec = crear_exec(afinidad_lista);
            //printf("Me creo un cocinero %s \n",afinidad_lista);
            char string_log[100];
            sprintf(string_log, "El cocinero de afinidad %s pasa a READY", afinidad_lista);
            log_info(logger, string_log);
            pushbacklist(&cola->cocineros, cola_exec);
            sem_post(cola->sem_cocinero_libre);
        
            
        }else{
            t_ready* cola = cola_por_afinidad("GENERAL");
            t_exec* cola_exec = crear_exec("GENERAL");
            //printf("Me creo un cocinero %s \n","GENERAL");
            char string_log[100];
            sprintf(string_log, "El cocinero de afinidad GENERAL pasa a READY");
            log_info(logger, string_log);
            pushbacklist(&cola->cocineros, cola_exec);
            sem_post(cola->sem_cocinero_libre);
        }
         
    }

}

t_ready* cola_por_afinidad(char* afinidad){

    for(IteratorList iter = beginlist(colas_ready); iter != NULL; iter = nextlist(iter)){
        t_ready* cola = (t_ready*) iter->data;

        if(!strcmp(cola->afinidad, afinidad)){
            return cola;
        }
    }
    return NULL;

}

void inicializar_colas_io(){

    for(int i=0;i<cantidad_hornos;i++){
        t_horno* horno = malloc(sizeof(t_horno));
        horno->ocupado = 0;
        horno->pcb = NULL;
        horno->sem_horno = malloc(sizeof(sem_t));
        sem_init(horno->sem_horno,0,0);
        char string_log[100];
        sprintf(string_log, "Se inicializa horno\n");
        log_info(logger, string_log);
        pushbacklist(&hornos,horno);
        sem_post(sem_horno_libre);
        pthread_t hilo_horno;
        pthread_create(&hilo_horno,NULL,(void*)paso_block,horno);
        pthread_detach(hilo_horno);


    }

}

void paso_exec(t_exec* cocinero){

    while(true){
        sem_wait(cocinero->semaforo_exec);
        sem_wait(cocinero->pcb->ciclo_cpu);
    
        if(!strcmp(restaurante_config->algoritmo_planificador,"FIFO"))
        {
            paso_a_exec_FIFO(cocinero);
        }
        else if(!strcmp(restaurante_config->algoritmo_planificador,"RR"))
        {
            paso_a_exec_RR(cocinero);
        }
    }
}


void paso_a_exec_FIFO(t_exec* cocinero){

    t_ready* cola = cola_por_afinidad(cocinero->afinidad);

    t_plato* plato = cocinero->pcb->plato;

    t_paso* paso = popfrontlist(&plato->pasos); 
    
    for(int i = 0; i<paso->ciclo_cpu; i++){ 
        sem_wait(cocinero->pcb->ciclo_cpu);
        printf("PCB %i - Ejecutando paso %s \n",cocinero->pcb->pid,paso->nombre_paso);
    }
    paso = frontlist(plato->pasos);

    if (paso == NULL){
        paso_exit(cocinero->pcb);
        pushbacklist(&cola->cocineros, cocinero);
        sem_post(cola->sem_cocinero_libre);
        return;
    } else if (!strcmp(paso->nombre_paso, "HORNEAR")){
        pushbacklist(&pcb_espera_horno, cocinero->pcb);
        sem_post(sem_block);
        pushbacklist(&cola->cocineros, cocinero);
        sem_post(cola->sem_cocinero_libre);
        return;
    }
    paso_ready(cocinero->pcb);
    pushbacklist(&cola->cocineros, cocinero);
    sem_post(cola->sem_cocinero_libre);

    
}


void paso_a_exec_RR(t_exec* cocinero){

    t_ready* cola = cola_por_afinidad(cocinero->afinidad);

    t_plato* plato = cocinero->pcb->plato;

    int quant = restaurante_config->quantum;

    t_paso* paso = frontlist(plato->pasos); //cambia para RR

    int i = 0;

    while(i < quant && paso != NULL && strcmp(paso->nombre_paso, "HORNEAR")){
        sem_wait(cocinero->pcb->ciclo_cpu);
        paso->ciclo_cpu -= 1;
        printf("PCB %i - Ejecutando paso %s \n",cocinero->pcb->pid,paso->nombre_paso);
        i++;
        if(paso->ciclo_cpu == 0)
        {
            popfrontlist(&plato->pasos);
            t_paso* proximo_paso = frontlist(plato->pasos); //cambia para RR

            if (proximo_paso == NULL){
                paso_exit(cocinero->pcb);
                pushbacklist(&cola->cocineros, cocinero);
                sem_post(cola->sem_cocinero_libre);
                return;
            } else if (!strcmp(proximo_paso->nombre_paso, "HORNEAR")){
                pushbacklist(&pcb_espera_horno, cocinero->pcb);
                sem_post(sem_block);
                pushbacklist(&cola->cocineros, cocinero);
                sem_post(cola->sem_cocinero_libre);
                return;
            } 
            
            paso_ready(cocinero->pcb);
            pushbacklist(&cola->cocineros, cocinero);
            sem_post(cola->sem_cocinero_libre);
        }
    }
    
    paso_ready(cocinero->pcb);
    pushbacklist(&cola->cocineros, cocinero);
    sem_post(cola->sem_cocinero_libre);
    
}

void controlador_hornos(){

    while(true){
        sem_wait(sem_block);
        sem_wait(sem_horno_libre);

        t_horno* horno = popfrontlist(&hornos);
        t_pcb* pcb_horno = popfrontlist(&pcb_espera_horno);

        horno->pcb = pcb_horno;
        horno->ocupado = 1;

        char string_log[100];
        sprintf(string_log, "El pcb %d pasa a BLOCK", pcb_horno->pid);
        log_info(logger, string_log);

        sem_post(horno->sem_horno);
    }

}

// VA A SER EL ALGORITMO

void controlador_colas_ready(){
    while(true){
       
        sem_wait(sem_ready);

        for(IteratorList iter = beginlist(colas_ready); iter != NULL; iter = nextlist(iter)){
            t_ready* cola = (t_ready*) iter->data;

            if(!isemptylist(cola->pcb_espera) && !isemptylist(cola->cocineros)){
                asignar_pcb_cocinero(cola);
                break;
            }
        }
    }

}

void controlador_ready(t_ready* cola){

    while(true){
        sem_wait(cola->sem_cocinero_libre);
        printf("SE RECIBIO COCINERO DE %s\n", cola->afinidad);
        sem_wait(cola->sem_pcb_espera);
        printf("SE RECIBIO PCB DE %s\n", cola->afinidad);
        controlador_a_ready(cola);
    }

}

void controlador_a_ready(t_ready* cola){

    sem_post(sem_ready);

}

void asignar_pcb_cocinero(t_ready* cola){

    t_pcb* pcb = popfrontlist(&cola->pcb_espera);
    t_exec* cocinero = popfrontlist(&cola->cocineros);

    cocinero->pcb = pcb;

    char string_log[100];
    sprintf(string_log, "El pcb %d pasa a EXEC\nSe le asigna al cocinero con afinidad %s\n", pcb->pid, cocinero->afinidad);
    log_info(logger, string_log);

    pushbacklist(&suscriptores_cpu, pcb->ciclo_cpu);
    sem_post(cocinero->semaforo_exec);
}

void paso_ready(t_pcb* pcb){


    t_ready* cola = cola_por_afinidad(pcb->afinidad);
    char string_log[100];
    sprintf(string_log, "El pcb %d pasa a READY", pcb->pid);
    log_info(logger, string_log);
    desuscribirse_clock(pcb->ciclo_cpu);
    pushbacklist(&cola->pcb_espera, pcb);
    sem_post(cola->sem_pcb_espera);

}

// int paso_ready(t_pcb* pcb){   

//     t_ready* cola_ready = cola_ready_pcb(pcb);
//     pushbacklist(&cola_ready->pcb_espera,pcb);
//     pcb->estado = READY;
//     printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));

    
//     for(IteratorList iter_cocinero = beginlist(colas_exec); iter_cocinero != NULL; iter_cocinero = nextlist(iter_cocinero)){
//         t_exec* cocinero = iter_cocinero->data;

//         if(!strcmp(pcb->plato->nombre, cocinero->afinidad)){

//             printf("Le tiro el post \n");

//             sem_post(cocinero->semaforo_exec);
            
//         }else if(plato_general(pcb->plato->nombre) && !strcmp(cocinero->afinidad,"GENERAL")){

//             printf("Le tiro el post en general \n");

//             sem_post(cocinero->semaforo_exec);

//         }


//         printf(" -%s \n",cocinero->afinidad);

//     }

    
//    return 1;

// }



// int plato_general(char* nombre_plato){

//     for(IteratorList iter_afinidad = beginlist(afinidades); iter_afinidad != NULL; iter_afinidad = nextlist(iter_afinidad)){
//         char* afinidad = iter_afinidad->data;

//         if(!strcmp(afinidad,nombre_plato)){
//             return 0;
//         }

//     }

//     return 1;
// }







int paso_exit(t_pcb* pcb){

    pushbacklist(&colas_exit,pcb->plato);
    pcb->estado = EXIT;

    printf(" - El plato %s esta en estado %s \n",pcb->plato->nombre, obtener_estado(pcb->estado));
     
    return 1;
};


void paso_block(t_horno* horno){

    while(true){
        sem_wait(horno->sem_horno);
        sem_wait(horno->pcb->ciclo_cpu);
        paso_a_block(horno);
    }

}
void paso_a_block(t_horno* horno){

    t_paso* paso = popfrontlist(&horno->pcb->plato->pasos);
    
    for(int i = 0; i < paso->ciclo_cpu; i++){
        sem_wait(horno->pcb->ciclo_cpu);
        printf("HORNO %i - Paso: %s \n",horno->pcb->pid,   paso->nombre_paso);
    }

    horno->ocupado = 0;
    pushbacklist(&hornos,horno);
    sem_post(sem_horno_libre);

    paso_ready(horno->pcb);

}



// int termino_pedido(int id_pedido){

//     for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
        
//         t_pcb* pcb = iter_pcb->data;

//         if(pcb->id_pedido == id_pedido && pcb->estado == EXIT){
//             return 1;
//         }

//     }

//     return 0;

// }



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


int es_paso_io(t_paso* paso){

    if( !strcmp((paso->nombre_paso),"HORNEAR") || !strcmp((paso->nombre_paso),"Hornear") || !strcmp((paso->nombre_paso),"Reposar")){
        return 1;
    }

   return 0;
}





void planificacion(){

    pthread_t planificar_hornos;
    pthread_create(&planificar_hornos, NULL, (void*) controlador_hornos, NULL);
    pthread_detach(planificar_hornos);

    iniciar_clock();


    controlador_colas_ready();

    // printf("Estoy en planificacion \n");

    // for(IteratorList iter_pcb = beginlist(colas_pcb); iter_pcb != NULL; iter_pcb = nextlist(iter_pcb)){
    //     t_pcb* pcb = iter_pcb->data;

    //     if(pcb->estado == NEW){
    //         paso_ready(pcb);
    //     }


    // }printf("---------- \n");


    // for(IteratorList iter_cocinero = beginlist(colas_exec); iter_cocinero != NULL; iter_cocinero = nextlist(iter_cocinero)){
    //     t_exec* cocinero = iter_cocinero->data;

    //     printf(" -%s \n",cocinero->afinidad);



    // }printf("---------- \n");


}




t_paso* crear_paso(char* nombre_paso, int ciclo_cpu){

    t_paso* paso = malloc(sizeof(paso));
     
    paso->nombre_paso = nombre_paso;
    paso->ciclo_cpu = ciclo_cpu;


    return paso;

}


t_plato* crear_plato(char* nombre, List* pasos, int pedido_id, int cantidad_total, int cantidad_listo,int pid){

    t_plato* plato = malloc(sizeof(t_plato));

    plato->nombre = nombre;
    plato->pasos = *pasos;
    plato->cantidad_total = cantidad_total;
    plato->cantidad_listo = cantidad_listo;

    crear_pcb(pedido_id,pid,plato, nombre);


    return plato;

}


t_pcb* crear_pcb(int id_pedido,int pid,t_plato* plato, char* afinidad){

    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->pid = pid;
    pcb->id_pedido = id_pedido;
    pcb->afinidad = string_new();
    string_append(&pcb->afinidad, afinidad_por_nombre_plato(afinidad));
    printf("AFINIDAD DEVUELTA: %s\n", afinidad_por_nombre_plato(afinidad));
    pcb->estado = NEW;
    pcb->plato = plato;    
    pcb->ciclo_cpu = malloc(sizeof(sem_t));
    sem_init(pcb->ciclo_cpu, 0 ,0);

    paso_ready(pcb);
    //pushfrontlist(&colas_pcb,pcb);


    return pcb;
}

t_pedido* creacion_pedido(int id_pedido, List* platos){

    t_pedido* pedido = malloc(sizeof(t_pedido));
    pedido->id_pedido = id_pedido;
    pedido->platos = *platos;


    return pedido;

}


t_exec* crear_exec(char* afinidad){

    t_exec* exec = malloc(sizeof(t_exec));
    exec->ocupado = 0;
    exec->pcb = NULL;
    exec->afinidad = string_new();
    string_append(&exec->afinidad, afinidad);
    exec->semaforo_exec = malloc(sizeof(sem_t));
    sem_init((exec->semaforo_exec),0,0);
    pthread_t hilo_ejecucion_exe;
    pthread_create(&hilo_ejecucion_exe,NULL,(void*)paso_exec,exec);
    pthread_detach(hilo_ejecucion_exe);

    return exec;
}

char* afinidad_por_nombre_plato(char* nombre){

    t_ready* ready = cola_por_afinidad(nombre);

    if(ready == NULL){
        return "GENERAL";
    } 
    return ready->afinidad;

}

void desuscribirse_clock(sem_t* ciclo_cpu){
    for (IteratorList it = beginlist(suscriptores_cpu); it != NULL ; it = nextlist(it)){
        if ((sem_t*)dataiterlist(it) == ciclo_cpu){
            popiterlist(&suscriptores_cpu, it);
            break;
        }
    }
}