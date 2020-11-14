#include "planificador.h"

void iniciarPlanificador(){
    
    initlist(&suscriptores_cpu);
    
    iniciarRepartidores();
}

void iniciarRepartidores(){
    
    for (int i = 0; app_config->repartidores[i] != NULL; i++){
        t_repartidor* repartidor_actual = malloc(sizeof(t_repartidor));

        char** posiciones_spliteadas = string_split(app_config->repartidores[i], "|");

        if (app_config->frecuencia_descanso[i] == NULL || app_config->tiempo_descanso[i] == NULL){
            //FALLO
        }

        repartidor_actual->frecuencia_de_descanso = atoi(app_config->frecuencia_descanso[i]);
        repartidor_actual->tiempo_de_descanso = atoi(app_config->tiempo_descanso[i]);
        repartidor_actual->posicion.posx = atoi(posiciones_spliteadas[0]);
        repartidor_actual->posicion.posy = atoi(posiciones_spliteadas[1]);
        repartidor_actual->cansancio = 0;
        repartidor_actual->nuevo_pedido = malloc(sizeof(sem_t));
        sem_init(repartidor_actual->nuevo_pedido, 0, 0);
        
    }
    
}