#include "sindicato.h"
int main(void){
//ESTE VA A ACTUAR DE SERVER
        t_log* logger = log_create("./cfg/sindicato.log", "SINDICATO", true, LOG_LEVEL_INFO);
        log_info(logger, "Soy el SINDICATO %s", mi_funcion_compartida());
        iniciar_servidor("127.0.0.1", "6011", handler);
        log_destroy(logger);
}

void handler(t_result* resultado){
        for (int i = 0; i < *resultado->mensajes->size; i++){
                printf("%s", resultado->mensajes->mensajes[i]);
        }
        printf("\n");
        return;
}