#include "sindicato.h"
int main(void){
//ESTE VA A ACTUAR DE SERVER
     t_log* logger = log_create("./cfg/sindicato.log", "SINDICATO", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el SINDICATO %s", mi_funcion_compartida());

     inicio_server("127.0.0.1", "6010",logger);



        log_destroy(logger);
        
}