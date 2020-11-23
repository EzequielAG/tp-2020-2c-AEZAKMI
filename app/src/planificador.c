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
        repartidor_actual->nuevo_pedido = malloc(sizeof(sem_t));
        sem_init(repartidor_actual->nuevo_pedido, 0, 0);
        // repartidor_actual->espera_pedido = malloc(sizeof(sem_t));
        // sem_init(repartidor_actual->espera_pedido, 0, 0);
        repartidor_actual->ciclo_cpu = malloc(sizeof(sem_t));
        sem_init(repartidor_actual->ciclo_cpu, 0, 0);

        pthread_t thread;
        pthread_create(&thread,NULL,(void*)repartir_pedidos, repartidor_actual);
        pthread_detach(thread);

        pushbacklist(&repartidores_libres, repartidor_actual);
        sem_post(sem_entrenador_libre);

        log_info(logger, "Se crea un repartidor");
        
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
        t_repartidor* repartidor = popfrontlist(&repartidores_libres);

        repartidor->pcb_actual = pcb;
        pcb->repartidor_actual = repartidor;

        log_info(logger, "Se asigna pcb a repartidor");

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

        log_info(logger, "Pasa a exec");
        
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

<<<<<<< HEAD
t_pcb* crear_pcb(char* restaurante, int id_pedido){

    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->restaurante = restaurante;
    pcb->id_pedido = id_pedido;
    
    return pcb;
}
=======

>>>>>>> f93f143a96b2906ae41ea05ef2a8847547675181
