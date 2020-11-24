#include "planificador.h"

void iniciar_planificador(){
    
    initlist(&suscriptores_cpu);
    initlist(&repartidores_libres);
    initlist(&pcb_new);
    initlist(&pcb_ready);

    sem_entrenador_libre = malloc(sizeof(sem_t));
    sem_init(sem_entrenador_libre, 0, 0);

    sem_pcb_new =  malloc(sizeof(sem_t));
    sem_init(sem_pcb_new, 0, 0);

    sem_pcb_ready =  malloc(sizeof(sem_t));
    sem_init(sem_pcb_ready, 0, 0);

    sem_grado_multiprocesamiento = malloc(sizeof(sem_t));
    sem_init(sem_grado_multiprocesamiento, 0, app_config->grado_multiprocesamiento);
    
    iniciar_repartidores();

    //pcb_prueba();

    iniciar_planificador_largo_plazo();

    iniciar_clock();

    iniciar_planificador_corto_plazo();

}

void iniciar_clock(){
    pthread_t thread;
    pthread_create(&thread,NULL,(void*)clock_cpu, NULL);
	pthread_detach(thread);
}

void clock_cpu(){
    
    while(true){
        sleep(app_config->retardo_ciclo_cpu);

        log_info(logger, "-----------CICLO-------------");

        for (IteratorList il = beginlist(suscriptores_cpu); il != NULL; il = nextlist(il)){
            sem_t* suscriptor = (sem_t*) dataiterlist(il);
            sem_post(suscriptor);
        }
    }
}

void iniciar_repartidores(){
    
    for (int i = 0; app_config->repartidores[i] != NULL; i++){
        t_repartidor* repartidor_actual = malloc(sizeof(t_repartidor));

        char** posiciones_spliteadas = string_split(app_config->repartidores[i], "|");

        if (app_config->frecuencia_descanso[i] == NULL || app_config->tiempo_descanso[i] == NULL){
            printf("Tiempo o frecuencia de descanso mal inicializada\n");
            return;
        }

        repartidor_actual->id = i;
        repartidor_actual->frecuencia_de_descanso = atoi(app_config->frecuencia_descanso[i]);
        repartidor_actual->tiempo_de_descanso = atoi(app_config->tiempo_descanso[i]);
        repartidor_actual->posicion.posx = atoi(posiciones_spliteadas[0]);
        repartidor_actual->posicion.posy = atoi(posiciones_spliteadas[1]);
        repartidor_actual->cansancio = 0;
        sem_init(repartidor_actual->nuevo_pedido, 0, 0);
        repartidor_actual->nuevo_pedido = malloc(sizeof(sem_t));
        // repartidor_actual->espera_pedido = malloc(sizeof(sem_t));
        // sem_init(repartidor_actual->espera_pedido, 0, 0);
        repartidor_actual->ciclo_cpu = malloc(sizeof(sem_t));
        sem_init(repartidor_actual->ciclo_cpu, 0, 0);

        pthread_t thread;
        pthread_create(&thread,NULL,(void*)repartir_pedidos, repartidor_actual);
        pthread_detach(thread);

        pushbacklist(&repartidores_libres, repartidor_actual);
        sem_post(sem_entrenador_libre);

        char string_log[100];
        sprintf(string_log, "Se crea un repartidor %d", repartidor_actual->id);
        log_info(logger, string_log);
        
    }
    
}

void iniciar_planificador_largo_plazo(){
    pthread_t thread;
    pthread_create(&thread,NULL,(void*)planificar_largo_plazo, NULL);
	pthread_detach(thread);
}

void iniciar_planificador_corto_plazo(){
    if (!strcmp(app_config->algoritmo_planificacion, "FIFO")){
        planificar_corto_plazo_FIFO();
    }

    //TODO: SJF Y HRRN
}

void planificar_largo_plazo(){
    while (true){
        sem_wait(sem_pcb_new);
        sem_wait(sem_entrenador_libre);


        t_pcb* pcb = popfrontlist(&pcb_new);

        t_repartidor* repartidor = obtener_repartidor_mas_cercano(pcb);

        repartidor->pcb_actual = pcb;
        pcb->repartidor_actual = repartidor;

        char string_log[100];
        sprintf(string_log, "Se asigna el pcb %d al repartidor %d\n", pcb->id_pedido, repartidor->id);
        log_info(logger, string_log);

        sem_post(repartidor->nuevo_pedido);

        pushbacklist(&pcb_ready, pcb);
        sem_post(sem_pcb_ready);
    }
}

void planificar_corto_plazo_FIFO(){
    while (true){
        sem_wait(sem_pcb_ready);
        sem_wait(sem_grado_multiprocesamiento);

        t_pcb* pcb = popfrontlist(&pcb_ready);

        char string_log[100];
        sprintf(string_log, "PCB A EXEC: %d\n", pcb->id_pedido);
        log_info(logger, string_log);
        
        pushbacklist(&suscriptores_cpu, pcb->repartidor_actual->ciclo_cpu);

    }
}

void pcb_prueba(){
    
    pushbacklist(&pcb_new, crear_pcb("Resto Default", 1, NULL));
    sem_post(sem_pcb_new);
    pushbacklist(&pcb_new, crear_pcb("Resto Default", 2, NULL));
    sem_post(sem_pcb_new);
    pushbacklist(&pcb_new, crear_pcb("Resto Default", 3, NULL));
    sem_post(sem_pcb_new);
}

t_repartidor* obtener_repartidor_mas_cercano(t_pcb* pcb){

    t_repartidor* repartidorAux = NULL;
    int calculoAux;
    t_restaurante *restaurante = buscar_restaurante_lista(pcb->restaurante);
    
    for(IteratorList iter = beginlist(repartidores_libres); iter != NULL; iter = nextlist(iter)){

        t_repartidor* repartidor = (t_repartidor*) iter->data;
        
        int posix = repartidor->posicion.posx - restaurante->posicion.posx;

        int posiy = repartidor->posicion.posy - restaurante->posicion.posy;

        int calculo = abs(posix) + abs(posiy);

        if(repartidorAux == NULL){
            repartidorAux = repartidor;
            calculoAux = calculo;
        }
        if(calculo < calculoAux){
            repartidorAux = repartidor;
            calculoAux = calculo;
        }
    }
    return repartidorAux;
}

