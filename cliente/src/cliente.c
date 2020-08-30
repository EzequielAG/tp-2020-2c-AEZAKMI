#include "cliente.h"
int main(void){
//ESTE VA A ACTUAR DE SERVER
     t_log* logger = log_create("./cfg/cliente.log", "PROCESO1", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el CLIENTE! %s", mi_funcion_compartida());





        log_destroy(logger);
        
}