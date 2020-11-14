#include "planificador.h"

void iniciar_planificador(){
    
    initlist(&suscriptores_cpu);
    initlist(&repartidores_libres);
    initlist(&pcb_new);
    initlist(&pcb_ready);
    
    iniciar_repartidores();

    pcb_prueba();

    iniciar_planificador_largo_plazo();

    iniciar_planificador_corto_plazo();

}

void iniciar_repartidores(){
    
    for (int i = 0; app_config->repartidores[i] != NULL; i++){
        t_repartidor* repartidor_actual = malloc(sizeof(t_repartidor));

        char** posiciones_spliteadas = string_split(app_config->repartidores[i], "|");

        if (app_config->frecuencia_descanso[i] == NULL || app_config->tiempo_descanso[i] == NULL){
            //FALLO
        }

        repartidor_actual->id = i;
        repartidor_actual->frecuencia_de_descanso = atoi(app_config->frecuencia_descanso[i]);
        repartidor_actual->tiempo_de_descanso = atoi(app_config->tiempo_descanso[i]);
        repartidor_actual->posicion.posx = atoi(posiciones_spliteadas[0]);
        repartidor_actual->posicion.posy = atoi(posiciones_spliteadas[1]);
        repartidor_actual->cansancio = 0;
        repartidor_actual->nuevo_pedido = malloc(sizeof(sem_t));
        sem_init(repartidor_actual->nuevo_pedido, 0, 0);

        pushbacklist(&repartidores_libres, repartidor_actual);
        
    }
    
}

void iniciar_planificador_largo_plazo(){

}

void iniciar_planificador_corto_plazo(){

}

void pcb_prueba(){

    pushbacklist(&pcb_new, crear_pcb("Resto Default", 1));
    pushbacklist(&pcb_new, crear_pcb("Resto Default", 2));
    pushbacklist(&pcb_new, crear_pcb("Resto Default", 3));
}

t_pcb* crear_pcb(char* restaurante, int id_pedido){
    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->restaurante = restaurante;
    pcb->id_pedido = id_pedido;
    return pcb;
}
