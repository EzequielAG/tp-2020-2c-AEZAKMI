#include "cliente.h"
int main(void){
     t_log* logger = log_create("./cfg/cliente.log", "CLIENTE", true, LOG_LEVEL_INFO);
     log_info(logger, "Soy el CLIENTE! %s", mi_funcion_compartida());
    log_destroy(logger);
}