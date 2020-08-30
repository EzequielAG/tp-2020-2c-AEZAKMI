#include "app.h"
int main(void){
//ESTE VA A ACTUAR DE SERVER
     t_log* logger = log_create("./cfg/app.log", "APP", true, LOG_LEVEL_INFO);
    log_info(logger, "Soy el MODULO APP ! %s", mi_funcion_compartida());



    log_destroy(logger);
        
}